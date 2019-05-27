#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include "SerfChannel.h"
#include "SerfIoThread.h"


namespace SerfCpp {

    SerfIoThread::SerfIoThread():
        m_socket(-1), m_seq(0), m_shutdown(false)
    {
    }

    SerfIoThread::~SerfIoThread()
    {
        Close();
    }

    bool
    SerfIoThread::Connect(const std::string &ipAddr,
                          const short &port)
    {
        m_ipAddr = ipAddr;
        m_port = port;
        m_socket = socket(AF_INET, SOCK_STREAM, 0);

        if (m_socket != -1) {
            struct sockaddr_in server;
            server.sin_addr.s_addr=inet_addr(m_ipAddr.c_str());
            server.sin_family = AF_INET;
            server.sin_port=htons(m_port);

            if (connect(m_socket, (struct sockaddr*)&server,sizeof(server)) == 0) {
                // Set to non-blocking
                int flags = fcntl(m_socket,F_GETFL,0);
                fcntl(m_socket,F_SETFL, flags | O_NONBLOCK);

                
                // Connected to the serf agent, so start the I/O thread
                m_thread = std::thread(&SerfIoThread::processRpc, this, 0);

                // Send the handshake message
                RequestHeader hdr;
                HandshakeRequest handshake;
                hdr.Command="handshake";
                handshake.Version = SerfApiVersion;

                ResultChannel<bool> channel;
                unsigned long long seq = 0;

                if (sendData(hdr,handshake, &channel,seq)) {
                    channel.consume();

                    if (channel.m_dataPending) {
                        return (channel.m_hdr.Error == "");
                    } else { // Timeout, no response was received
                        removeChannel(seq);
                    }
                }
            } else {
                close(m_socket);
                m_socket=-1;
            }
        }
        return false;
    }

    bool
    SerfIoThread::IsConnected()
    {
        return (m_socket != -1);
    }

    bool
    SerfIoThread::Close()
    {
        if (IsConnected()) {
            m_shutdown = true;
            m_thread.join();
        }

        m_shutdown = false;
        m_seq = 0;
        return true;
    }

    void
    SerfIoThread::processRpc(int)
    {
        while (!m_shutdown) {
            fd_set read_flags, write_flags;
            struct timeval waitd;

            waitd.tv_sec = 0;
            waitd.tv_usec = 10000;
            FD_ZERO(&read_flags);
            FD_ZERO(&write_flags);
            FD_SET(m_socket, &read_flags);

            int sel = select(m_socket+1,&read_flags, &write_flags, NULL,&waitd);
            if (sel < 0) {
                continue;
            }

            if (FD_ISSET(m_socket,&read_flags)) {
                FD_CLR(m_socket,&read_flags);

                m_unpacker.reserve_buffer(4096);
                ssize_t count = read(m_socket,m_unpacker.buffer(),
                                     m_unpacker.buffer_capacity());
                if (count <= 0) {
                    if (count == 0) {
                        // Connection closed
                    }
                    if (errno == EAGAIN || errno == EINTR) {
                        continue;
                    }
                    // Other error
                }

                //                char *cur = m_unpacker.buffer();
                //                printf("Received:\n");
                //                for (int i = 0; i < count; ++i) {
                //                    printf("0x%02x,",(int)(*cur & 0xff));
                //                    cur++;
                //                }
                //                printf("\n");

                m_unpacker.buffer_consumed(count);


                msgpack::object_handle oh;

                while (m_unpacker.next(oh)) {
                    msgpack::object obj = oh.get();
                    try {
                        ResponseHeader hdr = obj.as<ResponseHeader>();

                        // std::cout << "Hdr:" << obj << std::endl;

                        ChannelBase *channel = NULL;
                        {
                            std::lock_guard<std::mutex> guard(m_mutex);
                            channel = m_channels[hdr.Seq];
                        }

                        if (channel != NULL) {
                            // Request channels need to be removed from the channel map
                            if (channel->m_type == ChannelBase::REQUEST) {
                                std::lock_guard<std::mutex> guard(m_mutex);
                                m_channels.erase(hdr.Seq);
                            }

                            // std::cout << "Signalling response for seq " << hdr.Seq << std::endl;
                            channel->produce(hdr,m_unpacker);
                        }
                    } catch (std::exception &e) {
                        std::cout << "Unexpected data received from serf agent" << std::endl;
                        continue;

                    }
                }
            }
        }
        // Close the socket and exit the thread
        close(m_socket);
    }

    //
    // Force template instantiations for request types called from SerfClient
    //
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, JoinRequest&, ResultChannel<JoinResponse>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, AuthRequest&, ResultChannel<bool>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, KeyRequest&, ResultChannel<bool>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, KeyRequest&, ResultChannel<KeyResponse>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, ResultChannel<KeyListResponse>*,unsigned long long &seq);
    
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, ResultChannel<MembersResponse>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, MembersFilteredRequest &, ResultChannel<MembersResponse>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, EventRequest&, ResultChannel<bool>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, ForceLeaveRequest&, ResultChannel<bool>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, TagsRequest&, ResultChannel<bool>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, QueryRequest&, ResultChannel<bool>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, ResultChannel<bool>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, MonitorRequest&, ResultChannel<bool>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, StreamRequest&, ResultChannel<bool>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, StopRequest&, ResultChannel<bool>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, RespondRequest&, ResultChannel<bool>*,unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, ResultChannel<StatsResponse>*, unsigned long long &seq);
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, CoordRequest&, ResultChannel<CoordResponse>*, unsigned long long &seq);

    template<typename T, typename C>
    bool SerfIoThread::sendData(RequestHeader &hdr, T &body, C *channel,unsigned long long &seq)
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        hdr.Seq = m_seq++;        
        std::stringstream ss;
        msgpack::pack(ss,hdr);
        msgpack::pack(ss,body);

        bool result = (send(m_socket,ss.str().data(),ss.str().size(),0) != -1);
        if (result) {
            m_channels[hdr.Seq] = channel;
            std::cout << "sendData() with Seq=" << hdr.Seq << std::endl;
            seq = hdr.Seq;
        }
        
        return result;
    }

    template<typename C>
    bool SerfIoThread::sendData(RequestHeader &hdr, C *channel,unsigned long long &seq)
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        hdr.Seq = m_seq++;        
        std::stringstream ss;
        msgpack::pack(ss,hdr);

        bool result = (send(m_socket,ss.str().data(),ss.str().size(),0) != -1);
        if (result) {
            m_channels[hdr.Seq] = channel;        
            std::cout << "sendData() with Seq=" << hdr.Seq << std::endl;
            seq = hdr.Seq;
        }

        return result;
    }

    void SerfIoThread::addLogChannel(const unsigned long long &seq, ISerfLogListener *listener)
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        m_channels[seq] = new LogChannel(listener);
    }

    void SerfIoThread::addEventChannel(const unsigned long long &seq, ISerfEventListener *listener)
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        m_channels[seq] = new EventChannel(listener);
    }

    void SerfIoThread::addQueryChannel(const unsigned long long &seq, ISerfQueryListener *listener)
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        m_channels[seq] = new QueryChannel(*this,listener);
    }

    void SerfIoThread::removeChannel(const unsigned long long &seq)
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        ChannelBase *chan = m_channels[seq];
        m_channels.erase(seq);

        // For dynamically allocated channels, delete the channel instance
        // as well
        if (chan && chan->m_type != ChannelBase::REQUEST) {
            delete chan;
        }
    }

} // namespace SerfCpp
