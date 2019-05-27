#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "serf-cpp/SerfMsgStructs.h"

namespace SerfCpp {

class ISerfLogListener;
class ISerfEventListener;
class ISerfQueryListener;
class SerfIoThread;

struct ChannelBase {
    enum ChannelType { REQUEST, LOG, EVENT, QUERY };

    ChannelBase(ChannelType type);

    virtual ~ChannelBase();

    virtual void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) = 0;

    ChannelType m_type;
};

struct LogChannel : public ChannelBase {
    LogChannel(ISerfLogListener *);

    ~LogChannel();

    void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker);

    ISerfLogListener *m_listener;
};

struct EventChannel : public ChannelBase {
    EventChannel(ISerfEventListener *);

    ~EventChannel();

    void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker);

    ISerfEventListener *m_listener;
};

struct QueryChannel : public ChannelBase {
    QueryChannel(SerfIoThread &, ISerfQueryListener *);

    ~QueryChannel();

    void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker);

    SerfIoThread &m_ioThread;
    ISerfQueryListener *m_listener;
};

template <typename T>
struct ResultChannel : ChannelBase {
    ResultChannel() : ChannelBase(ChannelBase::REQUEST), m_dataPending(false) {}
    void consume() {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_dataPending == false) {
            if (m_condition.wait_for(lock, std::chrono::seconds(5)) == std::cv_status::timeout) {
                // Timeout
                break;
            }
        }
        // Data is now available
    }

    void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_hdr = hdr;

            // Unpack the payload
            // msgpack::unpacked unp;
            msgpack::object_handle oh;
            if (unpacker.next(oh)) {
                msgpack::object obj = oh.get();
                m_data = obj.as<T>();
            }
            m_dataPending = true;
            m_condition.notify_one();
        }
    }

    ResponseHeader m_hdr;
    T m_data;
    bool m_dataPending;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};

template <>
struct ResultChannel<bool> : ChannelBase {
    ResultChannel() : ChannelBase(ChannelBase::REQUEST), m_dataPending(false) {}
    void consume() {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_dataPending == false) {
            if (m_condition.wait_for(lock, std::chrono::seconds(5)) == std::cv_status::timeout) {
                // Timeout
                break;
            }
        }
        // Data is now available
    }

    void produce(ResponseHeader &hdr, msgpack::unpacker &) {
        {
            // No payload to unpack
            std::lock_guard<std::mutex> lock(m_mutex);
            m_hdr = hdr;
            m_dataPending = true;
            m_condition.notify_one();
        }
    }

    ResponseHeader m_hdr;
    bool m_dataPending;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};

}  // namespace SerfCpp
