#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include "SerfIoThread.h"

namespace SerfCpp {

    ChannelBase::ChannelBase()
    {}

    ChannelBase::~ChannelBase()
    {
    }

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
                m_thread = boost::thread(&SerfIoThread::processRpc, this, 0);

                // Send the handshake message
                RequestHeader hdr;
                HandshakeRequest handshake;
                hdr.Command="handshake";
                handshake.Version = SerfApiVersion;

                ResultChannel<bool> channel;

                sendData(hdr,handshake, &channel);

                channel.consume();

                return (channel.m_hdr.Error == "");

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
        m_shutdown = true;
        m_thread.join();

        return true;
    }

    void
    SerfIoThread::processRpc(int arg)
    {
        while (!m_shutdown) {
            fd_set read_flags, write_flags;
            struct timeval waitd;

            waitd.tv_sec = 10;
            FD_ZERO(&read_flags);
            FD_ZERO(&write_flags);
            FD_SET(m_socket, &read_flags);

            int sel = select(m_socket+1,&read_flags, &write_flags, NULL,&waitd);
            if (sel < 0) {
                continue;
            }

            if (FD_ISSET(m_socket,&read_flags)) {
                FD_CLR(m_socket,&read_flags);

                m_unpacker.reserve_buffer(2048);
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
                m_unpacker.buffer_consumed(count);

                msgpack::unpacked result;
                if (m_unpacker.next(&result)) {
                    msgpack::object obj = result.get();
                    try {
                        ResponseHeader hdr = obj.as<ResponseHeader>();

                        std::cout << "Hdr:" << obj << std::endl;

                        ChannelBase *channel = m_channels[hdr.Seq];

                        if (channel != NULL) {
                            std::cout << "Signalling response for seq " << hdr.Seq << std::endl;
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
    SerfIoThread::sendData(RequestHeader &hdr, JoinRequest&, ResultChannel<JoinResponse>*);

    template bool
    SerfIoThread::sendData(RequestHeader &hdr, ResultChannel<MembersResponse>*);

    template bool
    SerfIoThread::sendData(RequestHeader &hdr, MembersFilteredRequest &, ResultChannel<MembersResponse>*);
    
    template bool
    SerfIoThread::sendData(RequestHeader &hdr, EventRequest&, ResultChannel<bool>*);

    template bool
    SerfIoThread::sendData(RequestHeader &hdr, ForceLeaveRequest&, ResultChannel<bool>*);

    template bool
    SerfIoThread::sendData(RequestHeader &hdr, TagsRequest&, ResultChannel<bool>*);
    

    template<typename T, typename C>
    bool SerfIoThread::sendData(RequestHeader &hdr, T &body, C *channel)
    {
        boost::lock_guard<boost::mutex> guard(m_sendMutex);

        hdr.Seq = m_seq++;

        m_channels[hdr.Seq] = channel;
        std::cout << "sendData() with sequence=" << hdr.Seq << std::endl;

        std::stringstream ss;
        msgpack::pack(ss,hdr);
        msgpack::pack(ss,body);

        bool result = (send(m_socket,ss.str().data(),ss.str().size(),0) == 0);

        return result;
    }

    template<typename C>
    bool SerfIoThread::sendData(RequestHeader &hdr, C *channel)
    {
        boost::lock_guard<boost::mutex> guard(m_sendMutex);

        hdr.Seq = m_seq++;
        std::cout << "sendData() with sequence=" << hdr.Seq << std::endl;
        m_channels[hdr.Seq] = channel;
        
        std::stringstream ss;
        msgpack::pack(ss,hdr);

        bool result = (send(m_socket,ss.str().data(),ss.str().size(),0) == 0);

        return result;
    }
        
    

} // namespace SerfCpp
