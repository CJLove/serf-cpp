#include <iostream>

#include "SerfMsgStructs.h"
#include "gtest/gtest.h"

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

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<AuthRequest>(),req);
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

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<MembersResponse>(),resp);
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

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<StreamRequest>(), req);
}

TEST(SerfMsgStructTest, MonitorRequest)
{
    MonitorRequest req;
    req.LogLevel="debug";
    
    std::stringstream ss;
    msgpack::pack(ss,req);
    printMsgPack(ss.str());

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

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<StopRequest>(), req);
}

TEST(SerfMsgStructTest, LogRecord)
{
    LogRecord rec;
    rec.Log = "Log message data";

    std::stringstream ss;
    msgpack::pack(ss,rec);
    printMsgPack(ss.str());

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

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<QueryRecord>(), rec);
}
    
    
    
    
    
    


