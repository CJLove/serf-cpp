#pragma once

#include "serf-cpp/SerfMsgStructs.h"

namespace SerfCpp {

class ISerfLogListener {
public:
    ISerfLogListener();

    virtual ~ISerfLogListener();

    virtual void onLogRecord(ResponseHeader &hdr, LogRecord &record) = 0;
};

class ISerfEventListener {
public:
    ISerfEventListener();

    virtual ~ISerfEventListener();

    virtual void onUserEventRecord(ResponseHeader &hdr, UserEventRecord &record) = 0;

    virtual void onMemberEventRecord(ResponseHeader &hdr, MemberEventRecord &record) = 0;

    virtual void onQueryEventRecord(ResponseHeader &hdr, QueryRecord &record) = 0;
};

class ISerfQueryListener {
public:
    ISerfQueryListener();

    virtual ~ISerfQueryListener();

    virtual void onQueryAck(ResponseHeader &hdr, NodeAck &resp) = 0;

    virtual void onQueryResponse(ResponseHeader &hdr, NodeResponse &resp) = 0;

    virtual void onQueryComplete(ResponseHeader &hdr) = 0;
};

}  // namespace SerfCpp
