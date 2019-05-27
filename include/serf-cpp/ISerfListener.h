#pragma once

#include "serf-cpp/SerfMsgStructs.h"

namespace SerfCpp {

class ISerfLogListener {
public:
    ISerfLogListener() = default;
    ISerfLogListener(const ISerfLogListener&) = delete;
    ISerfLogListener(const ISerfLogListener&&) = delete;

    virtual ~ISerfLogListener() = default;

    void operator=(const ISerfLogListener &) = delete;
    void operator=(const ISerfLogListener &&) = delete;

    virtual void onLogRecord(ResponseHeader &hdr, LogRecord &record) = 0;
};

class ISerfEventListener {
public:
    ISerfEventListener() = default;
    ISerfEventListener(const ISerfEventListener &) = delete;
    ISerfEventListener(const ISerfEventListener &&) = delete;

    virtual ~ISerfEventListener() = default;
    
    void operator=(const ISerfEventListener &) = delete;
    void operator=(const ISerfEventListener &&) = delete;

    virtual void onUserEventRecord(ResponseHeader &hdr, UserEventRecord &record) = 0;

    virtual void onMemberEventRecord(ResponseHeader &hdr, MemberEventRecord &record) = 0;

    virtual void onQueryEventRecord(ResponseHeader &hdr, QueryRecord &record) = 0;
};

class ISerfQueryListener {
public:
    ISerfQueryListener() = default;
    ISerfQueryListener(const ISerfQueryListener &) = delete;
    ISerfQueryListener(const ISerfQueryListener &&) = delete;

    virtual ~ISerfQueryListener() = default;

    void operator=(const ISerfQueryListener &) = delete;
    void operator=(const ISerfQueryListener &&) = delete;

    virtual void onQueryAck(ResponseHeader &hdr, NodeAck &resp) = 0;

    virtual void onQueryResponse(ResponseHeader &hdr, NodeResponse &resp) = 0;

    virtual void onQueryComplete(ResponseHeader &hdr) = 0;
};

}  // namespace SerfCpp
