#include <iostream>
#include "gtest/gtest.h"

#include "serf-cpp/ISerfListener.h"
#include "SerfIoThread.h"

using namespace SerfCpp;

class MyLogListener: public ISerfLogListener
{
public:
    MyLogListener(): m_count(0)
    {}

    ~MyLogListener() = default;

    void onLogRecord(SerfCpp::ResponseHeader &, const SerfCpp::LogRecord &record) override;

    int m_count;
    std::string m_log;
};

void
MyLogListener::onLogRecord(SerfCpp::ResponseHeader &, const SerfCpp::LogRecord &record)
{
    m_count++;
    m_log = record.Log;
}

class MyEventListener: public ISerfEventListener {
public:
    MyEventListener(): m_userCount(0),m_memberCount(0),m_queryCount(0)
    {}

    ~MyEventListener() = default;

    void onUserEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::UserEventRecord &record) override;

    void onMemberEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::MemberEventRecord &record) override;

    void onQueryEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::QueryRecord &record) override;

    int m_userCount;
    int m_memberCount;
    int m_queryCount;
};

void
MyEventListener::onUserEventRecord(SerfCpp::ResponseHeader &, SerfCpp::UserEventRecord &)
{
    m_userCount++;
}

void
MyEventListener::onMemberEventRecord(SerfCpp::ResponseHeader &, SerfCpp::MemberEventRecord &)
{
	m_memberCount++;
}

void
MyEventListener::onQueryEventRecord(SerfCpp::ResponseHeader &, SerfCpp::QueryRecord &)
{
	m_queryCount++;
}

class MyQueryListener: public ISerfQueryListener
{
public:
    MyQueryListener() = default;

    void onQueryAck(ResponseHeader &, NodeAck &) override;

    void onQueryResponse(ResponseHeader &, NodeResponse &) override;

    void onQueryComplete(ResponseHeader &) override;

};

void
MyQueryListener::onQueryAck(ResponseHeader &, NodeAck &)
{
}


void
MyQueryListener::onQueryResponse(ResponseHeader &, NodeResponse &)
{
}

void
MyQueryListener::onQueryComplete(ResponseHeader &)
{
}

TEST(SerfIoThreadTest, nonThreadTests)
{
    SerfIoThread io;

    ASSERT_EQ(io.IsConnected(),false);

    ASSERT_EQ(io.Close(),true);

    ISerfLogListener *logListener = new MyLogListener();
    ISerfEventListener *eventListener = new MyEventListener();
    ISerfQueryListener *queryListener = new MyQueryListener();

    uint64_t seq = 12345ULL;
    io.addLogChannel(seq,logListener);

    io.removeChannel(seq);

    seq = 666666ULL;
    io.addEventChannel(seq, eventListener);

    io.removeChannel(seq);

    seq = 44444ULL;
    io.addQueryChannel(seq, queryListener);

    io.removeChannel(seq);   

    delete logListener; 
    delete eventListener;
    delete queryListener;

}

TEST(SerfIoThreadTest, connectFail)
{
    SerfIoThread io;

    ASSERT_EQ(io.Connect("127.0.0.1",6000), false);
}
