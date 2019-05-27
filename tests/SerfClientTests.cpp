#include <iostream>
#include "gtest/gtest.h"

#include "serf-cpp/ISerfListener.h"
#include "serf-cpp/SerfClient.h"
#include "Listeners.h"

using namespace SerfCpp;



TEST(SerfClientTests, Connect)
{
    SerfClient client;

    SerfClient::SerfResponse resp = client.Connect("127.0.0.1",6000);
    ASSERT_EQ(resp, SerfClient::FAILURE);

    std::cout << resp << std::endl;

    ASSERT_EQ(client.IsConnected(),false);

    resp = client.Close();
    ASSERT_EQ(resp,SerfClient::SUCCESS);

    std::cout << resp << std::endl;

}

TEST(SerfClientTests, Join)
{
    SerfClient client;

    SerfCpp::SerfStringArray addrs;
    addrs.push_back("192.168.0.1");
    int nodeCount = 0;
    SerfClient::SerfResponse resp = client.Join(addrs,false,nodeCount);

    ASSERT_EQ(resp,SerfClient::FAILURE);

    std::cout << resp << std::endl;    
}

TEST(SerfClientTests, Auth)
{
    SerfClient client;

    std::string token = "string";
    SerfClient::SerfResponse resp = client.Auth(token);

    ASSERT_EQ(resp,SerfClient::FAILURE);
}

TEST(SerfClientTests, InstallKey)
{
    SerfClient client;

    std::string key = "Key string here";
    KeyResponse keys;

    SerfClient::SerfResponse resp = client.InstallKey(key, keys);

    ASSERT_EQ(resp,SerfClient::FAILURE);    
}

TEST(SerfClientTests, ListKeys)
{
    SerfClient client;

    KeyListResponse keys;

    SerfClient::SerfResponse resp = client.ListKeys(keys);    

    ASSERT_EQ(resp,SerfClient::FAILURE);    
}

TEST(SerfClientTests, UseKey)
{
    SerfClient client;

    std::string key="keyvalue";

    SerfClient::SerfResponse resp = client.UseKey(key);    

    ASSERT_EQ(resp,SerfClient::FAILURE);    
}

TEST(SerfClientTests, RemoveKey)
{
    SerfClient client;

    std::string key = "Key string here";
    KeyResponse keys;

    SerfClient::SerfResponse resp = client.RemoveKey(key, keys);

    ASSERT_EQ(resp,SerfClient::FAILURE);    
}
    
TEST(SerfClientTests, Members)
{
    SerfClient client;

    MembersResponse members;

    SerfClient::SerfResponse resp = client.Members(members);

    ASSERT_EQ(resp,SerfClient::FAILURE);
}

TEST(SerfClientTests, MembersFiltered)
{
    SerfClient client;

    SerfCpp::SerfStringMap tags;
    tags["abc"]="def";
    std::string status="running";
    std::string name="node1";
    MembersResponse members;

    SerfClient::SerfResponse resp = client.MembersFiltered(tags,status,name,members);

    ASSERT_EQ(resp,SerfClient::FAILURE);
}

TEST(SerfClientTests, Event)
{
    SerfClient client;

    std::string evtName="myEvent";
    SerfCpp::SerfPayload payload;
    bool coalesce=false;

    SerfClient::SerfResponse resp = client.Event(evtName,payload,coalesce);

    ASSERT_EQ(resp,SerfClient::FAILURE);
}

TEST(SerfClientTests, Response)
{
    SerfClient client;

    SerfCpp::SerfPayload payload;

    uint64_t id = 123ULL;

    SerfClient::SerfResponse resp = client.Respond(id, payload);
    
    ASSERT_EQ(resp,SerfClient::FAILURE);
}
    
TEST(SerfClientTests, ForceLeave)
{
    SerfClient client;

    std::string node = "node1";

    SerfClient::SerfResponse resp = client.ForceLeave(node);
    
    ASSERT_EQ(resp,SerfClient::FAILURE);
}

TEST(SerfClientTests, Tags)
{
    SerfClient client;

    SerfCpp::SerfStringMap tags;
    SerfCpp::SerfStringArray deleteTags;
    tags["abc"]="def";
    deleteTags.push_back("xyz");

    SerfClient::SerfResponse resp = client.Tags(tags,deleteTags);
    
    ASSERT_EQ(resp,SerfClient::FAILURE);
}

TEST(SerfClientTests, Leave)
{
    SerfClient client;

    SerfClient::SerfResponse resp = client.Leave();
    
    ASSERT_EQ(resp,SerfClient::FAILURE);
}

TEST(SerfClientTests, Monitor)
{
    SerfClient client;

    uint64_t seq;

    std::string level="debug";

    LogListener listener;

    SerfClient::SerfResponse resp = client.Monitor(level, &listener, seq);

    ASSERT_EQ(resp,SerfClient::FAILURE);    
}

TEST(SerfClientTests, Query)
{
    SerfClient client;

    std::string name = "queryName";
    QueryListener listener;
    SerfCpp::SerfPayload payload;

    SerfClient::SerfResponse resp = client.Query(name,payload,&listener);

    ASSERT_EQ(resp,SerfClient::FAILURE);    
}

TEST(SerfClientTests, Stream)
{
    SerfClient client;

    EventListener listener;

    std::string type="all";
    uint64_t seq;

    SerfClient::SerfResponse resp = client.Stream(type,&listener,seq);
    ASSERT_EQ(resp,SerfClient::FAILURE);    
}

TEST(SerfClientTests, Stop)
{
    SerfClient client;

    uint64_t seq = 12345ULL;

    SerfClient::SerfResponse resp = client.Stop(seq);
    ASSERT_EQ(resp,SerfClient::FAILURE);    
}

TEST(SerfClientTests, Stats)
{
    SerfClient client;

    StatsResponse stats;

    SerfClient::SerfResponse resp = client.Stats(stats);
    ASSERT_EQ(resp,SerfClient::FAILURE);    
}

TEST(SerfClientTests, GetCoordinate)
{
    SerfClient client;

    std::string node="node1";
    CoordResponse coord;
    SerfClient::SerfResponse resp = client.GetCoordinate(node,coord);
    ASSERT_EQ(resp,SerfClient::FAILURE);    
}
    
    
    
    
