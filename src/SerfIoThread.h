#pragma once
#include <string>

#include <boost/thread.hpp>

#include "SerfMsgStructs.h"

namespace SerfCpp {

    class ISerfLogListener;
    class ChannelBase;
    
    class SerfIoThread {
    public:
        SerfIoThread();

        ~SerfIoThread();

        bool Connect(const std::string &ipAddr, const short& port);

        bool IsConnected();

        bool Close();

        void processRpc(int arg);

        template<typename T, typename C>
            bool sendData(RequestHeader &hdr, T &body, C *channel);

        template<typename C>
            bool sendData(RequestHeader &hdr, C* channel);

        void addLogChannel(const unsigned long long &seq, ISerfLogListener *listener);

        void removeChannel(const unsigned long long &seq);

    private:
        // I/O thread for receiving data from serf agent
        boost::thread m_thread;

        // Mutex for sending data
        boost::mutex m_sendMutex;

        // Msgpack decoder
        msgpack::unpacker m_unpacker;

        // IP Address of Serf Agent
        std::string m_ipAddr;

        // Port of Serf Agent
        short m_port;

        // Socket for the agent connection
        int m_socket;

        // Sequence number for outgoing RPC messages
        unsigned long long m_seq;

        // Flag to shutdown
        bool m_shutdown;

        std::map<unsigned long long, ChannelBase*> m_channels;
    };

} // namespace SerfCpp
