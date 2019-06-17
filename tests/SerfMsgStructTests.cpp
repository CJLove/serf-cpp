#include <iostream>
#include "gtest/gtest.h"

#include "serf-cpp/SerfMsgStructs.h"


using namespace SerfCpp;

TEST(SerfMsgStructTest, RequestHeader)
{
    RequestHeader hdr;
    hdr.Command="handshake";
    hdr.Seq=0;

    std::stringstream ss;
    msgpack::pack(ss, hdr);
    printMsgPack(ss.str());

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<RequestHeader>(),hdr);
}

TEST(SerfMsgStructTest, ResponseHeader)
{
    ResponseHeader resp;
    resp.Seq = 5;
    resp.Error = "Error";

    std::stringstream ss;
    msgpack::pack(ss,resp);
    printMsgPack(ss.str());

    std::cout << resp << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<ResponseHeader>(),resp);
}
    
TEST(SerfMsgStructTest, HandshakeRequest)
{
    HandshakeRequest req;
    req.Version = 5;

    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<HandshakeRequest>(),req);
}

TEST(SerfMsgStructTest, AuthRequest)
{
    AuthRequest req;
    req.AuthKey="TheKey";

    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    std::cout << req << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<AuthRequest>(),req);
}

TEST(SerfMsgStructTest, KeyResponse)
{
    KeyResponse resp;
    resp.NumErr = 1;
    resp.NumNodes = 2;
    resp.NumResp = 3;
    resp.Messages["abc"] = "def";

    std::stringstream ss;
    msgpack::pack(ss,resp);
    printMsgPack(ss.str());

    std::cout << resp << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<KeyResponse>(),resp);
}

TEST(SerfMsgStructTest, CoordRessponse)
{
    CoordResponse resp;
    resp.Ok=false;
    resp.Coord.Vec.push_back(2.4);
    resp.Coord.Vec.push_back(4.8);
    resp.Coord.Error = 6.432;
    resp.Coord.Adjustment = 542.1;
    resp.Coord.Height = 12345.6;

    std::stringstream ss;
    msgpack::pack(ss,resp);
    printMsgPack(ss.str());

    std::cout << resp << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<CoordResponse>(),resp);
}
    
    

TEST(SerfMsgStructTest, KeyListResponse)
{
    KeyListResponse resp;
    resp.Messages["abc"] = "def";
    resp.Keys["efg"] = 1;
    resp.Keys["hij"] = 2;
    resp.NumErr = 1;
    resp.NumNodes = 2;
    resp.NumResp = 3;

    std::stringstream ss;
    msgpack::pack(ss,resp);
    printMsgPack(ss.str());

    std::cout << resp << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<KeyListResponse>(),resp);
}
    
TEST(SerfMsgStructTest, KeyRequest)
{
    KeyRequest req;
    req.Key="TheKey";

    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    std::cout << req << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<KeyRequest>(),req);
}

TEST(SerfMsgStructTest, EventRequest)
{
    EventRequest req;
    req.Name="MyEvent";
    req.Payload.push_back('T');
    req.Payload.push_back('E');
    req.Payload.push_back('S');
    req.Payload.push_back('T');    
    req.Coalesce=true;

    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<EventRequest>(),req);
}
    
TEST(SerfMsgStructTest, ForceLeaveRequest)
{
    ForceLeaveRequest req;
    req.Node="20.1.2.3";
    
    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<ForceLeaveRequest>(),req);
}

TEST(SerfMsgStructTest, JoinRequest)
{
    JoinRequest req;
    req.Existing.push_back("20.1.2.3:6000");
    req.Existing.push_back("20.1.2.4:6000");
    req.Replay = false;
    
    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<JoinRequest>(),req);
}

TEST(SerfMsgStructTest, JoinResponse)
{
    JoinResponse resp;
    resp.Num=66;

    std::stringstream ss;
    msgpack::pack(ss,resp);
    printMsgPack(ss.str());

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<JoinResponse>(),resp);
}

TEST(SerfMsgStructTest, MembersResponse)
{
    MembersResponse resp;
    Member m;
    m.Name="chasm";
    m.Addr.push_back(127);
    m.Addr.push_back(0);
    m.Addr.push_back(0);
    m.Addr.push_back(1);
    m.Port=5000;
    m.Status="alive";
    m.ProtocolMin = m.ProtocolMax = m.ProtocolCur = m.DelegateMin
        = m.DelegateMax = m.DelegateCur = 1;
    resp.Members.push_back(m);
    m.Name="fir";
    resp.Members.push_back(m);
    
    std::stringstream ss;
    msgpack::pack(ss,resp);
    printMsgPack(ss.str());

    std::cout << resp << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<MembersResponse>(),resp);
}

TEST(SerfMsgStructTest, StatsResponse)
{
    StatsResponse resp;
    resp.agent.name="nasdf";
    resp.runtime.os="Linux";
    resp.runtime.arch="x86_64";
    resp.runtime.version="0.7";
    resp.runtime.max_procs="2";
    resp.runtime.goroutines="4";
    resp.runtime.cpu_count="2";
    resp.serf.failed="0";
    resp.serf.left="1";
    resp.serf.event_time="2";
    resp.serf.query_time="3";
    resp.serf.event_queue="4";    
    resp.serf.members="5";    
    resp.serf.member_time="6";
    resp.serf.intent_queue="7";
    resp.serf.query_queue="8";

    std::stringstream ss;
    msgpack::pack(ss,resp);
    printMsgPack(ss.str());

    std::cout << resp << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<StatsResponse>(),resp);
}
    
    
TEST(SerfMsgStructTest, MembersFilteredRequest)
{
    MembersFilteredRequest req;

    req.Tags["tag1"]="value1";
    req.Tags["tag2"]="value2";
    req.Status = "running";
    req.Name = "node2";

    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    std::cout << req << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<MembersFilteredRequest>(), req);
}

TEST(SerfMsgStructTest, TagsRequest)
{
    TagsRequest req;
    req.Tags["tag1"]="value1";
    req.Tags["tag2"]="value2";

    req.DeleteTags.push_back("tag3");

    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    std::cout << req << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<TagsRequest>(), req);
}

TEST(SerfMsgStructTest, StreamRequest)
{
    StreamRequest req;
    req.Type="member-join";
    
    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    std::cout << req << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<StreamRequest>(), req);
}

TEST(SerfMsgStructTest, RespondRequest)
{
    RespondRequest req;
    req.ID=1234566ULL;
    req.Payload.push_back('c');    
    
    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    std::cout << req << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<RespondRequest>(), req);
}


TEST(SerfMsgStructTest, MonitorRequest)
{
    MonitorRequest req;
    req.LogLevel="debug";
    
    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    std::cout << req << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<MonitorRequest>(), req);
}

TEST(SerfMsgStructTest, StopRequest)
{
    StopRequest req;
    req.Stop = 12345;
    
    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    std::cout << req << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<StopRequest>(), req);
}

TEST(SerfMsgStructTest, CoordRequest)
{
    CoordRequest req;
    req.Node = "nodename";
    
    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

    std::cout << req << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<CoordRequest>(), req);
}

TEST(SerfMsgStructTest, NodeResponseBase)
{
    NodeResponseBase resp;
    resp.Type="None";
    std::stringstream ss;
    msgpack::pack(ss,resp);
    printMsgPack(ss.str());

    std::cout << resp << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<NodeResponseBase>(), resp);
}

TEST(SerfMsgStructTest, NodeAck)
{
    NodeAck resp;
    resp.Type="ack";
    resp.From="node1";    
    std::stringstream ss;
    msgpack::pack(ss,resp);
    printMsgPack(ss.str());

    std::cout << resp << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<NodeAck>(), resp);
}

TEST(SerfMsgStructTest, NodeResponse)
{
    NodeResponse resp;
    resp.Type="response";
    resp.From="node1";
    resp.Payload.push_back('z');
    
    std::stringstream ss;
    msgpack::pack(ss,resp);
    printMsgPack(ss.str());

    std::cout << resp << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<NodeResponse>(), resp);
}

TEST(SerfMsgStructTest, LogRecord)
{
    LogRecord rec;
    rec.Log = "Log message data";

    std::stringstream ss;
    msgpack::pack(ss,rec);
    printMsgPack(ss.str());

    std::cout << rec << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<LogRecord>(), rec);
}

TEST(SerfMsgStructTest, UserEventRecord)
{
    UserEventRecord rec;
    rec.Event="user-event";
    rec.LTime = 2354ULL;
    rec.Name = "my-event";
    rec.Payload.push_back('c');
    rec.Coalesce = true;

    std::stringstream ss;
    msgpack::pack(ss,rec);
    printMsgPack(ss.str());

    std::cout << rec << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<UserEventRecord>(), rec);
}

TEST(SerfMsgStructTest, MemberEventRecord)
{
    MemberEventRecord rec;
    rec.Event="member-join";
    Member m;
    m.Name="chasm";
    m.Addr.push_back(127);
    m.Addr.push_back(0);
    m.Addr.push_back(0);
    m.Addr.push_back(1);
    m.Port=5000;
    m.Status="alive";
    m.ProtocolMin = m.ProtocolMax = m.ProtocolCur = m.DelegateMin
        = m.DelegateMax = m.DelegateCur = 1;
    rec.Members.push_back(m);
    m.Name="fir";
    rec.Members.push_back(m);

    std::stringstream ss;
    msgpack::pack(ss,rec);
    printMsgPack(ss.str());

    std::cout << rec << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<MemberEventRecord>(), rec);
}
    
TEST(SerfMsgStructTest, QueryRecord)
{
    QueryRecord rec;
    rec.Event="query";
    rec.ID = 6666ULL;
    rec.LTime = 444ULL;
    rec.Name="test-event";
    rec.Payload.push_back('a');
    rec.Payload.push_back('b');
    rec.Payload.push_back('c');

    std::stringstream ss;
    msgpack::pack(ss,rec);
    printMsgPack(ss.str());

    std::cout << rec << std::endl;

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<QueryRecord>(), rec);
}
    
    
    
    
    
    


