#include <iostream>
#include "gtest/gtest.h"

#include "SerfIoThread.h"
#include "SerfChannel.h"
#include "Listeners.h"

using namespace SerfCpp;



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
    rec.Coalesce = false;
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

TEST(SerfChannelTest, ResultChannel)
{
    ResultChannel<bool> channel;
    ResponseHeader hdr;    

    EXPECT_EQ(channel.m_dataPending,false);

    channel.consume();
    EXPECT_EQ(channel.m_dataPending,false);    

    msgpack::unpacker unpacker;
    unpacker.reserve_buffer(4096);

    channel.produce(hdr,unpacker);

    EXPECT_EQ(channel.m_dataPending,true);

    channel.consume();

    EXPECT_EQ(channel.m_dataPending,true);    
    
}

TEST(SerfChannelTest, ResultChannelType)
{
    JoinResponse resp;
    resp.Num = 5;
    ResponseHeader hdr;
    ResultChannel<JoinResponse> channel;

    EXPECT_EQ(channel.m_dataPending,false);

    std::stringstream ss;
    msgpack::pack(ss,resp);
    msgpack::unpacker unpacker;
    unpacker.reserve_buffer(4096);
    memcpy(unpacker.buffer(),ss.str().data(),ss.str().size());
    unpacker.buffer_consumed(ss.str().size());

    channel.consume();
    EXPECT_EQ(channel.m_dataPending,false);    

    channel.produce(hdr,unpacker);

    EXPECT_EQ(channel.m_dataPending,true);        
    
    channel.consume();

    EXPECT_EQ(channel.m_dataPending,true);        

    EXPECT_EQ(channel.m_data.Num,5);

}

TEST(SerfChannelTest, QueryChannel)
{
    SerfIoThread io;
    uint64_t seq = 555;
	NodeAck resp;
    resp.Type="ack";
    NodeResponse resp2;
    resp2.Type="response";
    NodeResponseBase resp3;
    resp3.Type="done";
    
    QueryListener listener;
    QueryChannel channel(io,&listener);
    io.addQueryChannel(seq,&listener);
    ResponseHeader hdr;
    hdr.Seq = seq;
    
    std::stringstream ss;
    msgpack::pack(ss,resp);
    msgpack::unpacker unpacker;
    unpacker.reserve_buffer(4096);
    memcpy(unpacker.buffer(),ss.str().data(),ss.str().size());
    unpacker.buffer_consumed(ss.str().size());

    EXPECT_EQ(listener.m_queryAck,0);

    channel.produce(hdr,unpacker);

    EXPECT_EQ(listener.m_queryAck,1);

    {
        std::stringstream ss2;
	    msgpack::pack(ss2,resp2);
        msgpack::unpacker unpacker2;
        unpacker2.reserve_buffer(4096);    
	    memcpy(unpacker2.buffer(),ss2.str().data(),ss2.str().size());
        unpacker2.buffer_consumed(ss2.str().size());

        channel.produce(hdr,unpacker2);
    }

    EXPECT_EQ(listener.m_queryResp,1);    

    {
        std::stringstream ss3;
	    msgpack::pack(ss3,resp3);
        msgpack::unpacker unpacker3;
        unpacker3.reserve_buffer(4096);    
	    memcpy(unpacker3.buffer(),ss3.str().data(),ss3.str().size());
        unpacker3.buffer_consumed(ss3.str().size());

        channel.produce(hdr,unpacker3);
    }

    EXPECT_EQ(listener.m_queryComplete,1);    
    

    // nothing in unpacker
    
    channel.produce(hdr,unpacker);

    EXPECT_EQ(listener.m_queryAck,1);
    EXPECT_EQ(listener.m_queryResp,1);
    EXPECT_EQ(listener.m_queryComplete,1);    

    
}



