#pragma once
#include <string>

#include <boost/thread.hpp>

namespace SerfCpp {

class SerfIoThread {
public:
    SerfIoThread(const std::string &ioAddr,
                 const short &port);

    ~SerfIoThread();

    bool Connect();

    bool IsConnected();

    bool Close();

    void processRpc(int arg);
        


private:
    boost::thread m_thread;

    // IP Address of Serf Agent
    std::string m_ipAddr;

    // Port of Serf Agent
    short m_port;

    // Socket for the agent connection
    int m_socket;

    // Sequence number for outgoing RPC messages
    unsigned long m_seq;

    // Flag to shutdown
    bool m_shutdown;
};

} // namespace SerfCpp
