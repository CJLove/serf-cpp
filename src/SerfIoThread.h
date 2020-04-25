#pragma once
#include <string>

#include <mutex>
#include <thread>

#include "serf-cpp/SerfMsgStructs.h"

namespace SerfCpp {

class ISerfLogListener;
class ISerfEventListener;
class ISerfQueryListener;
struct ChannelBase;

class SerfIoThread {
public:
    SerfIoThread() = default;

    ~SerfIoThread();

    SerfIoThread(const SerfIoThread &) = delete;
    SerfIoThread(const SerfIoThread &&) = delete;

    void operator=(const SerfIoThread &) = delete;
    void operator=(const SerfIoThread &&) = delete;

    bool Connect(const std::string &ipAddr, const uint16_t &port);

    bool IsConnected();

    bool Close();

    void processRpc(int arg);

    template <typename T, typename C>
    bool sendData(RequestHeader &hdr, T &body, C *channel, uint64_t &seq);

    template <typename C>
    bool sendData(RequestHeader &hdr, C *channel, uint64_t &seq);

    void addLogChannel(const uint64_t &seq, ISerfLogListener *listener);

    void addEventChannel(const uint64_t &seq, ISerfEventListener *listener);

    void addQueryChannel(const uint64_t &seq, ISerfQueryListener *listener);

    void removeChannel(const uint64_t &seq);

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
    uint16_t m_port = 0;

    // Socket for the agent connection
    int m_socket = -1;

    // Sequence number for outgoing RPC messages
    uint64_t m_seq = 0;

    // Flag to shutdown
    bool m_shutdown = false;

    std::map<uint64_t, ChannelBase *> m_channels;
};

}  // namespace SerfCpp
