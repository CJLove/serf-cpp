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

constexpr int TIMEOUT_SECONDS = 5;

struct ChannelBase {
    enum ChannelType { REQUEST, LOG, EVENT, QUERY };

    explicit ChannelBase(ChannelType type);

    virtual ~ChannelBase() = default;

    ChannelBase(const ChannelBase &) = delete;
    ChannelBase(const ChannelBase &&) = delete;

    void operator=(const ChannelBase &) = delete;
    void operator=(const ChannelBase &&) = delete;

    virtual void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) = 0;

    ChannelType m_type;
};

struct LogChannel : public ChannelBase {
    explicit LogChannel(ISerfLogListener * /*listener*/);

    ~LogChannel() override = default;

    LogChannel(const LogChannel &) = delete;
    LogChannel(const LogChannel &&) = delete;

    void operator=(const LogChannel &) = delete;
    void operator=(const LogChannel &&) = delete;

    void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) override;

    ISerfLogListener *m_listener;
};

struct EventChannel : public ChannelBase {
    explicit EventChannel(ISerfEventListener * /*listener*/);

    ~EventChannel() override = default;

    EventChannel(const EventChannel &) = delete;
    EventChannel(const EventChannel &&) = delete;

    void operator=(const EventChannel &) = delete;
    void operator=(const EventChannel &&) = delete;

    void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) override;

    ISerfEventListener *m_listener;
};

struct QueryChannel : public ChannelBase {
    QueryChannel(SerfIoThread & /*iothread*/, ISerfQueryListener * /*listener*/);

    ~QueryChannel() override = default;

    QueryChannel(const QueryChannel &) = delete;
    QueryChannel(const QueryChannel &&) = delete;

    void operator=(const QueryChannel &) = delete;
    void operator=(const QueryChannel &&) = delete;

    void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) override;

    SerfIoThread &m_ioThread;
    ISerfQueryListener *m_listener;
};

template <typename T>
struct ResultChannel : ChannelBase {
    ResultChannel() : ChannelBase(ChannelBase::REQUEST) {}

    ~ResultChannel() override = default;

    ResultChannel(const ResultChannel &) = delete;
    ResultChannel(const ResultChannel &&) = delete;

    void operator=(const ResultChannel &) = delete;
    void operator=(const ResultChannel &&) = delete;

    void consume() {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_dataPending == false) {
            if (m_condition.wait_for(lock, std::chrono::seconds(TIMEOUT_SECONDS)) == std::cv_status::timeout) {
                // Timeout
                break;
            }
        }
        // Data is now available
    }

    void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) override {
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
    bool m_dataPending = false;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};

template <>
struct ResultChannel<bool> : ChannelBase {
    ResultChannel() : ChannelBase(ChannelBase::REQUEST) {}

    ~ResultChannel() override = default;

    ResultChannel(const ResultChannel &) = delete;
    ResultChannel(const ResultChannel &&) = delete;

    void operator=(const ResultChannel &) = delete;
    void operator=(const ResultChannel &&) = delete;

    void consume() {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (!m_dataPending) {
            if (m_condition.wait_for(lock, std::chrono::seconds(TIMEOUT_SECONDS)) == std::cv_status::timeout) {
                // Timeout
                break;
            }
        }
        // Data is now available
    }

    void produce(ResponseHeader &hdr, msgpack::unpacker & /*unpacker*/) override {
        {
            // No payload to unpack
            std::lock_guard<std::mutex> lock(m_mutex);
            m_hdr = hdr;
            m_dataPending = true;
            m_condition.notify_one();
        }
    }

    ResponseHeader m_hdr;
    bool m_dataPending = false;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};

}  // namespace SerfCpp
