#pragma once

#include "serf-cpp/SerfMsgStructs.h"

namespace SerfCpp {

/**
 * @brief ISerfLogListener is an interface for client code to register for callbacks when a serf log message is received
 *
 */
class ISerfLogListener {
public:
    /**
     * @brief Constructors
     *
     */
    ISerfLogListener() = default;
    ISerfLogListener(const ISerfLogListener &) = delete;
    ISerfLogListener(const ISerfLogListener &&) = delete;

    /**
     * @brief Destructor
     *
     */
    virtual ~ISerfLogListener() = default;

    /**
     * @brief Assignment operators removed
     *
     */
    void operator=(const ISerfLogListener &) = delete;
    void operator=(const ISerfLogListener &&) = delete;

    /**
     * @brief Callback for serf log records
     *
     * @param hdr - RPC message header
     * @param record - log record
     */
    virtual void onLogRecord(ResponseHeader &hdr, const LogRecord &record) = 0;
};

/**
 * @brief ISerfEventListener is an interface for client code to be notified of serf events
 *
 */
class ISerfEventListener {
public:
    /**
     * @brief Constructors
     *
     */
    ISerfEventListener() = default;
    ISerfEventListener(const ISerfEventListener &) = delete;
    ISerfEventListener(const ISerfEventListener &&) = delete;

    /**
     * @brief Destructor
     *
     */
    virtual ~ISerfEventListener() = default;

    /**
     * @brief Assignment operators
     *
     */
    void operator=(const ISerfEventListener &) = delete;
    void operator=(const ISerfEventListener &&) = delete;

    /**
     * @brief Callback method for user events
     *
     * @param hdr - RPC message header
     * @param record - user event record
     */
    virtual void onUserEventRecord(ResponseHeader &hdr, UserEventRecord &record) = 0;

    /**
     * @brief Callback method for member events
     *
     * @param hdr - RPC message header
     * @param record - member event record
     */
    virtual void onMemberEventRecord(ResponseHeader &hdr, MemberEventRecord &record) = 0;

    /**
     * @brief Callback method for query events
     *
     * @param hdr - RPC message header
     * @param record - query event record
     */
    virtual void onQueryEventRecord(ResponseHeader &hdr, QueryRecord &record) = 0;
};

/**
 * @brief ISerfQueryListener is an interface class for client code to receive callbacks related to Query requests
 *
 */
class ISerfQueryListener {
public:
    /**
     * @brief Constructors
     *
     */
    ISerfQueryListener() = default;
    ISerfQueryListener(const ISerfQueryListener &) = delete;
    ISerfQueryListener(const ISerfQueryListener &&) = delete;

    /**
     * @brief Destructors
     *
     */
    virtual ~ISerfQueryListener() = default;

    /**
     * @brief Assignment operators
     *
     */
    void operator=(const ISerfQueryListener &) = delete;
    void operator=(const ISerfQueryListener &&) = delete;

    /**
     * @brief Callback method for a query acknowledgement
     *
     * @param hdr - RPC message header
     * @param resp - acknowledgement record
     */
    virtual void onQueryAck(ResponseHeader &hdr, NodeAck &resp) = 0;

    /**
     * @brief Callback method for a query response
     *
     * @param hdr - RPC message header
     * @param resp - query response record
     */
    virtual void onQueryResponse(ResponseHeader &hdr, NodeResponse &resp) = 0;

    /**
     * @brief Callback method for a query completion
     *
     * @param hdr - RPC message header
     */
    virtual void onQueryComplete(ResponseHeader &hdr) = 0;
};

}  // namespace SerfCpp
