#include <iostream>

#include "SerfChannel.h"
#include "ISerfListener.h"
#include "gtest/gtest.h"

using namespace SerfCpp;

class LogListener: public ISerfLogListener
{
public:
    LogListener(): m_count(0)
    {}

    ~LogListener()
    {}

    void onLogRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::LogRecord &record);

    int m_count;
    std::string m_log;
};

void
LogListener::onLogRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::LogRecord &record)
{
    m_count++;
    m_log = record.Log;
}

class EventListener: public ISerfEventListener {
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

void
EventListener::onUserEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::UserEventRecord &record)
{
    m_userCount++;
}

void
EventListener::onMemberEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::MemberEventRecord &record)
{
	m_memberCount++;
}

void
EventListener::onQueryEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::QueryRecord &record)
{
	m_queryCount++;
}



TEST(SerfChannelTest, LogChannel)
{
    LogRecord rec;
    rec.Log="This is a log message";
    LogListener listener;
    LogChannel channel(&listener);
    ResponseHeader hdr;

    std::stringstream ss;
    msgpack::pack(ss,rec);
    msgpack::unpacker unpacker;
    unpacker.reserve_buffer(4096);
    memcpy(unpacker.buffer(),ss.str().data(),ss.str().size());
    unpacker.buffer_consumed(ss.str().size());

    EXPECT_EQ(listener.m_count,0);

    channel.produce(hdr,unpacker);

    EXPECT_EQ(listener.m_count,1);
    EXPECT_EQ(listener.m_log,rec.Log);

    // Nothing in unpacker
    channel.produce(hdr,unpacker);
    
    EXPECT_EQ(listener.m_count,2);
    EXPECT_EQ(listener.m_log,"");
}

TEST(SerfChannelTest, UserEventChannel)
{
    UserEventRecord rec;
    rec.Event="user";
    EventListener listener;
    EventChannel channel(&listener);
    ResponseHeader hdr;
    
    std::stringstream ss;
    msgpack::pack(ss,rec);
    msgpack::unpacker unpacker;
    unpacker.reserve_buffer(4096);
    memcpy(unpacker.buffer(),ss.str().data(),ss.str().size());
    unpacker.buffer_consumed(ss.str().size());

    EXPECT_EQ(listener.m_userCount,0);
    EXPECT_EQ(listener.m_memberCount,0);
    EXPECT_EQ(listener.m_queryCount,0);

    channel.produce(hdr,unpacker);

    EXPECT_EQ(listener.m_userCount,1);
    EXPECT_EQ(listener.m_memberCount,0);
    EXPECT_EQ(listener.m_queryCount,0);

    // nothing in unpacker
    
    channel.produce(hdr,unpacker);

    EXPECT_EQ(listener.m_userCount,1);
    EXPECT_EQ(listener.m_memberCount,0);
    EXPECT_EQ(listener.m_queryCount,0);
}

TEST(SerfChannelTest, MemberEventChannel)
{
    MemberEventRecord rec;
    rec.Event="member-join";
    EventListener listener;
    EventChannel channel(&listener);
    ResponseHeader hdr;
    
    std::stringstream ss;
    msgpack::pack(ss,rec);
    msgpack::unpacker unpacker;
    unpacker.reserve_buffer(4096);
    memcpy(unpacker.buffer(),ss.str().data(),ss.str().size());
    unpacker.buffer_consumed(ss.str().size());

    EXPECT_EQ(listener.m_userCount,0);
    EXPECT_EQ(listener.m_memberCount,0);
    EXPECT_EQ(listener.m_queryCount,0);

    channel.produce(hdr,unpacker);

    EXPECT_EQ(listener.m_userCount,0);
    EXPECT_EQ(listener.m_memberCount,1);
    EXPECT_EQ(listener.m_queryCount,0);

    // nothing in unpacker
    
    channel.produce(hdr,unpacker);

    EXPECT_EQ(listener.m_userCount,0);
    EXPECT_EQ(listener.m_memberCount,1);
    EXPECT_EQ(listener.m_queryCount,0);
}

TEST(SerfChannelTest, QueryEventChannel)
{
    QueryRecord rec;
    rec.Event="query";
    EventListener listener;
    EventChannel channel(&listener);
    ResponseHeader hdr;
    
    std::stringstream ss;
    msgpack::pack(ss,rec);
    msgpack::unpacker unpacker;
    unpacker.reserve_buffer(4096);
    memcpy(unpacker.buffer(),ss.str().data(),ss.str().size());
    unpacker.buffer_consumed(ss.str().size());

    EXPECT_EQ(listener.m_userCount,0);
    EXPECT_EQ(listener.m_memberCount,0);
    EXPECT_EQ(listener.m_queryCount,0);

    channel.produce(hdr,unpacker);

    EXPECT_EQ(listener.m_userCount,0);
    EXPECT_EQ(listener.m_memberCount,0);
    EXPECT_EQ(listener.m_queryCount,1);

    // nothing in unpacker
    
    channel.produce(hdr,unpacker);

    EXPECT_EQ(listener.m_userCount,0);
    EXPECT_EQ(listener.m_memberCount,0);
    EXPECT_EQ(listener.m_queryCount,1);
}


