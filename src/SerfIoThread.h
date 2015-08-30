#pragma once
#include <string>

#include <boost/thread.hpp>

#include "SerfMsgStructs.h"

namespace SerfCpp {

struct ChannelBase
{
    enum ChannelType { REQUEST, LOG, EVENT };
    
    ChannelBase(ChannelType type);

    virtual ~ChannelBase();

    virtual void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) = 0;

    ChannelType m_type;
};

template<typename T> struct ResultChannel: ChannelBase {
     ResultChannel(): ChannelBase(ChannelBase::REQUEST), m_dataPending(false)
     {}
     void consume() {
         boost::unique_lock<boost::mutex> lock(m_mutex);
         while (m_dataPending == false) {
             m_condition.wait(lock);
         }
         // Data is now available
     }

     void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) {
         {
             boost::lock_guard<boost::mutex> lock(m_mutex);
             m_hdr = hdr;

             // Unpack the payload
             msgpack::unpacked unp;
             if (unpacker.next(&unp)) {
                 msgpack::object obj = unp.get();
                 m_data = obj.as<T>();
             }
             m_dataPending = true;             
             m_condition.notify_one();
         }
     }

     ResponseHeader m_hdr;
     T m_data;
     bool m_dataPending;
     boost::mutex m_mutex;
     boost::condition_variable m_condition;
};

template<>
struct ResultChannel<bool>: ChannelBase {
     ResultChannel(): ChannelBase(ChannelBase::REQUEST), m_dataPending(false)
     {}
     void consume() {
         boost::unique_lock<boost::mutex> lock(m_mutex);
         while (m_dataPending == false) {
             m_condition.wait(lock);
         }
         // Data is now available
     }

     void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) {
         {
             // No payload to unpack
             boost::lock_guard<boost::mutex> lock(m_mutex);
             m_hdr = hdr;
             m_dataPending = true;             
             m_condition.notify_one();
         }
     }

     ResponseHeader m_hdr;
     bool m_dataPending;
     boost::mutex m_mutex;
     boost::condition_variable m_condition;
}; 
    
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
