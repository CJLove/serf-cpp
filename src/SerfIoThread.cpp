#include "SerfIoThread.h"
#include "SerfChannel.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

static const size_t RPC_BUFFER_SIZE = 1024 * 1024;  // 1MByte

namespace SerfCpp {

SerfIoThread::~SerfIoThread() { Close(); }

bool SerfIoThread::Connect(const std::string &ipAddr, const uint16_t &port) {
    m_ipAddr = ipAddr;
    m_port = port;
    m_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (m_socket != -1) {
        struct sockaddr_in server = {0,0,{0},{0}};  // Ensure all (4) fields are zero-initialized
        server.sin_addr.s_addr = inet_addr(m_ipAddr.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(m_port);

        if (connect(m_socket, reinterpret_cast<struct sockaddr *>(&server), sizeof(server)) == 0) {  // NOLINT clang-tidy ignore
            // Set to non-blocking
            int flags = fcntl(m_socket, F_GETFL, 0);
            fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);

            // Connected to the serf agent, so start the I/O thread
            m_thread = std::thread(&SerfIoThread::processRpc, this, 0);

            // Send the handshake message
            RequestHeader hdr;
            HandshakeRequest handshake;
            hdr.Command = "handshake";
            handshake.Version = SerfApiVersion;

            ResultChannel<bool> channel;
            uint64_t seq = 0;

            if (sendData(hdr, handshake, &channel, seq)) {
                channel.consume();

                if (channel.m_dataPending) {
                    return (channel.m_hdr.Error.empty());
                } else {  // Timeout, no response was received
                    removeChannel(seq);
                }
            }
        } else {
            close(m_socket);
            m_socket = -1;
        }
    }
    return false;
}

bool SerfIoThread::IsConnected() { return (m_socket != -1); }

bool SerfIoThread::Close() {
    if (IsConnected()) {
        m_shutdown = true;
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    m_shutdown = false;
    m_seq = 0;
    return true;
}

void SerfIoThread::processRpc(int /* arg */) {
    const int64_t FIFTY_MS = 50000; 
    while (!m_shutdown) {
        fd_set read_flags;
        fd_set write_flags;
        struct timeval waitd {
            0, 0
        };
        waitd.tv_sec = 0;
        waitd.tv_usec = FIFTY_MS;
        size_t count = 0;

        while (true) {
            FD_ZERO(&read_flags);
            FD_ZERO(&write_flags);
            FD_SET(m_socket, &read_flags);

            if (select(m_socket + 1, &read_flags, &write_flags, nullptr, &waitd) <= 0) {
                break;
            }

            if (!FD_ISSET(m_socket, &read_flags)) {
                break;
            }

            FD_CLR(m_socket, &read_flags);
            m_unpacker.reserve_buffer(RPC_BUFFER_SIZE);

            ssize_t bytesRead = read(m_socket, m_unpacker.buffer() + count, RPC_BUFFER_SIZE - count);

            if (bytesRead <= 0) {
                break;
            }

            count += static_cast<size_t>(bytesRead);
        }

        if (count > RPC_BUFFER_SIZE) {
            std::cout << "Error: RPC buffer size is too small for this response " << std::endl;
        }

        if (count <= 0) {
            if (count == 0) {
                // Connection closed
            }
            if (errno == EAGAIN || errno == EINTR) {
                continue;
            }
            // Other error
        }

        m_unpacker.buffer_consumed(count);

        msgpack::object_handle oh;

        while (m_unpacker.next(oh)) {
            msgpack::object obj = oh.get();
            try {
                ResponseHeader hdr = obj.as<ResponseHeader>();

                // std::cout << "Hdr:" << obj << std::endl;

                ChannelBase *channel = nullptr;
                {
                    std::lock_guard<std::mutex> guard(m_mutex);
                    channel = m_channels[hdr.Seq];
                }

                if (channel != nullptr) {
                    // Request channels need to be removed from the channel map
                    if (channel->m_type == ChannelBase::REQUEST) {
                        std::lock_guard<std::mutex> guard(m_mutex);
                        m_channels.erase(hdr.Seq);
                    }

                    // std::cout << "Signalling response for seq " << hdr.Seq << std::endl;
                    channel->produce(hdr, m_unpacker);
                }
            } catch (std::exception &e) {
                std::cout << "Unexpected data received from serf agent" << std::endl;
                continue;
            }
        }
    }

    // Close the socket and exit the thread
    close(m_socket);
}

//
// Force template instantiations for request types called from SerfClient
//
template bool SerfIoThread::sendData(RequestHeader &hdr, JoinRequest &, ResultChannel<JoinResponse> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, AuthRequest &, ResultChannel<bool> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, KeyRequest &, ResultChannel<bool> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, KeyRequest &, ResultChannel<KeyResponse> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, ResultChannel<KeyListResponse> *, uint64_t &seq);

template bool SerfIoThread::sendData(RequestHeader &hdr, ResultChannel<MembersResponse> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, MembersFilteredRequest &, ResultChannel<MembersResponse> *,
                                     uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, EventRequest &, ResultChannel<bool> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, ForceLeaveRequest &, ResultChannel<bool> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, TagsRequest &, ResultChannel<bool> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, QueryRequest &, ResultChannel<bool> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, ResultChannel<bool> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, MonitorRequest &, ResultChannel<bool> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, StreamRequest &, ResultChannel<bool> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, StopRequest &, ResultChannel<bool> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, RespondRequest &, ResultChannel<bool> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, ResultChannel<StatsResponse> *, uint64_t &seq);
template bool SerfIoThread::sendData(RequestHeader &hdr, CoordRequest &, ResultChannel<CoordResponse> *, uint64_t &seq);

template <typename T, typename C>
bool SerfIoThread::sendData(RequestHeader &hdr, T &body, C *channel, uint64_t &seq) {
    std::lock_guard<std::mutex> guard(m_mutex);
    hdr.Seq = m_seq++;
    std::stringstream ss;
    msgpack::pack(ss, hdr);
    msgpack::pack(ss, body);

    bool result = (send(m_socket, ss.str().data(), ss.str().size(), 0) != -1);
    if (result) {
        m_channels[hdr.Seq] = channel;
        // std::cout << "sendData() with Seq=" << hdr.Seq << std::endl;
        seq = hdr.Seq;
    }

    return result;
}

template <typename C>
bool SerfIoThread::sendData(RequestHeader &hdr, C *channel, uint64_t &seq) {
    std::lock_guard<std::mutex> guard(m_mutex);

    hdr.Seq = m_seq++;
    std::stringstream ss;
    msgpack::pack(ss, hdr);

    bool result = (send(m_socket, ss.str().data(), ss.str().size(), 0) != -1);
    if (result) {
        m_channels[hdr.Seq] = channel;
        // std::cout << "sendData() with Seq=" << hdr.Seq << std::endl;
        seq = hdr.Seq;
    }

    return result;
}

void SerfIoThread::addLogChannel(const uint64_t &seq, ISerfLogListener *listener) {
    std::lock_guard<std::mutex> guard(m_mutex);

    m_channels[seq] = new LogChannel(listener);
}

void SerfIoThread::addEventChannel(const uint64_t &seq, ISerfEventListener *listener) {
    std::lock_guard<std::mutex> guard(m_mutex);

    m_channels[seq] = new EventChannel(listener);
}

void SerfIoThread::addQueryChannel(const uint64_t &seq, ISerfQueryListener *listener) {
    std::lock_guard<std::mutex> guard(m_mutex);

    m_channels[seq] = new QueryChannel(*this, listener);
}

void SerfIoThread::removeChannel(const uint64_t &seq) {
    std::lock_guard<std::mutex> guard(m_mutex);

    ChannelBase *chan = m_channels[seq];
    m_channels.erase(seq);

    // For dynamically allocated channels, delete the channel instance
    // as well
    if ((chan != nullptr) && chan->m_type != ChannelBase::REQUEST) {
        delete chan; //NOLINT - chan requires deletion in non-REQUEST cases
    }
}

}  // namespace SerfCpp
