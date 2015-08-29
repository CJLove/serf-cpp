#include "SerfIoThread.h"

namespace SerfCpp {

    SerfIoThread::SerfIoThread(const std::string &ipAddr,
                               const short &port):
        m_ipAddr(ipAddr), m_port(port), m_socket(-1), m_seq(0),
        m_shutdown(false)
    {
    }

    SerfIoThread::~SerfIoThread()
    {
        Close();
    }

    bool
    SerfIoThread::Connect()
    {
    
    
        m_thread = boost::thread(&SerfIoThread::processRpc, this, 0);
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

            int i = 0;

        

        }
    }

} // namespace SerfCpp
