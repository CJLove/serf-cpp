#pragma once
#include <string>

#include <mutex>
#include <thread>

#include "serf-cpp/SerfMsgStructs.h"

namespace SerfCpp {

class ISerfLogListener;
class ISerfEventListener;
class ISerfQueryListener;
class ChannelBase;

class SerfIoThread {
public:
    SerfIoThread();

    ~SerfIoThread();

    bool Connect(const std::string &ipAddr, const short &port);

    bool IsConnected();

    bool Close();

    void processRpc(int arg);

    template <typename T, typename C>
    bool sendData(RequestHeader &hdr, T &body, C *channel, unsigned long long &seq);

    template <typename C>
    bool sendData(RequestHeader &hdr, C *channel, unsigned long long &seq);

    void addLogChannel(const unsigned long long &seq, ISerfLogListener *listener);

    void addEventChannel(const unsigned long long &seq, ISerfEventListener *listener);

    void addQueryChannel(const unsigned long long &seq, ISerfQueryListener *listener);

    void removeChannel(const unsigned long long &seq);

private:
    // I/O thread for receiving data from serf agent
    std::thread m_thread;

    // Mutex for sending data and channel data
    std::mutex m_mutex;

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

    std::map<unsigned long long, ChannelBase *> m_channels;
};

}  // namespace SerfCpp
