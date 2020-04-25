#pragma once
#include "serf-cpp/ISerfListener.h"

class LogListener: public SerfCpp::ISerfLogListener
{
public:
    LogListener(): m_count(0)
    {}

    ~LogListener()
    {}

    void onLogRecord(SerfCpp::ResponseHeader &hdr, const SerfCpp::LogRecord &record);

    int m_count;
    std::string m_log;
};

class EventListener: public SerfCpp::ISerfEventListener {
public:
    EventListener(): m_userCount(0),m_memberCount(0),m_queryCount(0)
    {}

    ~EventListener()
    {}

    void onUserEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::UserEventRecord &record);

    void onMemberEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::MemberEventRecord &record);

    void onQueryEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::QueryRecord &record);

    int m_userCount;
    int m_memberCount;
    int m_queryCount;
};

class QueryListener: public SerfCpp::ISerfQueryListener {
public:
    QueryListener(): m_queryAck(0),m_queryResp(0), m_queryComplete(0)
    {}

    void onQueryAck(SerfCpp::ResponseHeader &hdr, SerfCpp::NodeAck &resp);

    void onQueryResponse(SerfCpp::ResponseHeader &hdr, SerfCpp::NodeResponse &resp);

    void onQueryComplete(SerfCpp::ResponseHeader &hdr);
public:
    int m_queryAck;
    int m_queryResp;
    int m_queryComplete;
};
