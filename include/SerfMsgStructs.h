#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>
#define MSGPACK_USE_DEFINE_MAP
#include <msgpack.hpp>

namespace SerfCpp {

    const int SerfApiVersion = 1;

    // --------------------------
    struct RequestHeader {
        std::string Command;
        unsigned long Seq;
        MSGPACK_DEFINE(Command,Seq);
    };
    inline bool operator==(RequestHeader const& lhs, RequestHeader const& rhs) {
        return ((lhs.Command == rhs.Command) && (lhs.Seq == rhs.Seq));
    }

    // --------------------------    
    struct ResponseHeader {
        unsigned long long Seq;
        std::string Error;
        MSGPACK_DEFINE(Seq,Error);
    };
    inline bool operator==(ResponseHeader const& lhs, ResponseHeader const& rhs) {
        return ((lhs.Seq == rhs.Seq) && (lhs.Error == rhs.Error));
    }

    std::ostream &operator<<(std::ostream &os, const ResponseHeader &m);     

    // --------------------------    
    struct HandshakeRequest {
        int Version;
        MSGPACK_DEFINE(Version);
    };
    inline bool operator==(HandshakeRequest const& lhs, HandshakeRequest const& rhs) {
        return (lhs.Version == rhs.Version);
    }

    // --------------------------    
    struct AuthRequest {
        std::string AuthKey;
        MSGPACK_DEFINE(AuthKey);
    };
    inline bool operator==(AuthRequest const &lhs, AuthRequest const& rhs) {
        return (lhs.AuthKey == rhs.AuthKey);
    }

    // --------------------------    
    struct EventRequest {
        std::string Name;
        std::vector<signed char> Payload;
        bool Coalesce;
        MSGPACK_DEFINE(Name,Payload,Coalesce);
    };
    inline bool operator==(EventRequest const& lhs, EventRequest const& rhs) {
        return ((lhs.Name == rhs.Name) && (lhs.Payload == rhs.Payload) && (lhs.Coalesce == rhs.Coalesce));
    }

    // --------------------------    
    struct ForceLeaveRequest {
        std::string Node;
        MSGPACK_DEFINE(Node);
    };
    inline bool operator==(ForceLeaveRequest const& lhs, ForceLeaveRequest const& rhs) {
        return (lhs.Node == rhs.Node);
    }

    // --------------------------    
    struct JoinRequest {
        std::vector<std::string> Existing;
        bool Replay;
        MSGPACK_DEFINE(Existing,Replay);
    };
    inline bool operator==(JoinRequest const& lhs, JoinRequest const& rhs) {
        return ((lhs.Existing == rhs.Existing) && (lhs.Replay == rhs.Replay));
    }

    // --------------------------    
    struct JoinResponse {
        int Num;
        MSGPACK_DEFINE(Num);
    };
    inline bool operator==(JoinResponse const& lhs, JoinResponse const& rhs) {
        return (lhs.Num == rhs.Num);
    }

    // --------------------------    
    struct Member {
        std::string Name;
        // In the serf agent, the Go net.ip type is encoded as an array
        // of 16 bytes (covering IPv6 or IPv4 addr).
        // For reference: http://golang.org/pkg/net/#IP
        //
        // For ipv6 address, all 16 bytes are populated
        // For ipv4 address, last 4 bytes seem to be populated
        //
        // While 'char' is used to match the msgpack encoding, values should
        // be interpreted as unsigned char.
        std::vector<char> Addr;
        int Port;
        std::map<std::string,std::string> Tags;
        std::string Status;
        unsigned char ProtocolMin;
        unsigned char ProtocolMax;
        unsigned char ProtocolCur;
        unsigned char DelegateMin;
        unsigned char DelegateMax;
        unsigned char DelegateCur;

        MSGPACK_DEFINE(Name,Addr,Port,Tags,Status,
                       ProtocolMin,ProtocolMax,ProtocolCur,
                       DelegateMin,DelegateMax,DelegateCur);
    };
    inline bool operator==(Member const &lhs, Member const &rhs) {
        return ((lhs.Name == rhs.Name) &&
                (lhs.Addr == rhs.Addr) &&
                (lhs.Port == rhs.Port) &&
                (lhs.Tags == rhs.Tags) &&
                (lhs.Status == rhs.Status) &&
                (lhs.ProtocolMin == rhs.ProtocolMin) &&
                (lhs.ProtocolMax == rhs.ProtocolMax) &&
                (lhs.ProtocolCur == rhs.ProtocolCur) &&
                (lhs.DelegateMin == rhs.DelegateMin) &&
                (lhs.DelegateMax == rhs.DelegateMax) &&
                (lhs.DelegateMin == rhs.DelegateMin));
    }

    std::ostream &operator<<(std::ostream &os, const Member &m); 
             
    // --------------------------
    struct MembersResponse {
        std::vector<Member> Members;
        MSGPACK_DEFINE(Members);
    };
    inline bool operator==(MembersResponse const &lhs, MembersResponse const& rhs) {
        return (lhs.Members == rhs.Members);
    }
    std::ostream &operator<<(std::ostream &os, const MembersResponse &m);

    // -------------------------- 
    struct MembersFilteredRequest {
        std::map<std::string,std::string> Tags;
        std::string Status;
        std::string Name;
        MSGPACK_DEFINE(Tags,Status,Name);
    };
    inline bool operator==(MembersFilteredRequest const &lhs, MembersFilteredRequest const &rhs)
    {
        return (lhs.Tags == rhs.Tags &&
                lhs.Status == rhs.Status &&
                lhs.Name == rhs.Name);
    }
        
    std::ostream &operator<<(std::ostream &os, const MembersFilteredRequest &m);
    
    // --------------------------
    struct TagsRequest {
        std::map<std::string,std::string> Tags;
        std::vector<std::string> DeleteTags;
        MSGPACK_DEFINE(Tags,DeleteTags);
    };
    inline bool operator==(TagsRequest const &lhs, TagsRequest const &rhs)
    {
        return (lhs.Tags == rhs.Tags &&
                lhs.DeleteTags == rhs.DeleteTags);
    }

    std::ostream &operator<<(std::ostream &os, const TagsRequest &r);
    
    // --------------------------
    struct StreamRequest {
        std::string Type;
        MSGPACK_DEFINE(Type);
    };

    inline bool operator==(StreamRequest const &lhs, StreamRequest const &rhs)
    {
        return (lhs.Type == rhs.Type);
    }

    std::ostream &operator<<(std::ostream &os, const StreamRequest &r);
    
    // --------------------------
    struct MonitorRequest {
        std::string LogLevel;
        MSGPACK_DEFINE(LogLevel);
    };

    inline bool operator==(MonitorRequest const &lhs, MonitorRequest const &rhs)
    {
        return (lhs.LogLevel == rhs.LogLevel);
    }

    std::ostream &operator<<(std::ostream &os, const MonitorRequest &r);

    // --------------------------    
    struct StopRequest {
        unsigned long long Stop;
        MSGPACK_DEFINE(Stop);
    };

    inline bool operator==(StopRequest const &lhs, StopRequest const &rhs)
    {
        return (lhs.Stop == rhs.Stop);
    }

    std::ostream &operator<<(std::ostream &os, const StopRequest &r);

    // --------------------------    
    struct LogRecord {
        std::string Log;
        MSGPACK_DEFINE(Log);
    };

    inline bool operator==(LogRecord const &lhs, LogRecord const &rhs)
    {
        return (lhs.Log == rhs.Log);
    }

    std::ostream &operator<<(std::ostream &os, const LogRecord &r);

    // --------------------------    
    struct UserEventRecord {
        std::string Event;
        unsigned long long LTime;
        std::string Name;
        std::vector<char> Payload;
        bool Coalesce;
        MSGPACK_DEFINE(Event,LTime,Name,Payload,Coalesce);
    };

    inline bool operator==(UserEventRecord const &lhs, UserEventRecord const &rhs)
    {
        return (lhs.Event == rhs.Event &&
                lhs.LTime == rhs.LTime &&
                lhs.Name == rhs.Name &&
                lhs.Payload == rhs.Payload &&
                lhs.Coalesce == rhs.Coalesce);
    }

    std::ostream &operator<<(std::ostream &os, const UserEventRecord &m);     

    // --------------------------    
    struct MemberEventRecord {
        std::string Event;
        std::vector<Member> Members;
        MSGPACK_DEFINE(Event,Members);
    };

    inline bool operator==(MemberEventRecord const &lhs, MemberEventRecord const &rhs)
    {
        return (lhs.Event == rhs.Event &&
                lhs.Members == rhs.Members);
    }

    std::ostream &operator<<(std::ostream &os, const MemberEventRecord &m);         

    // --------------------------    
    struct QueryRecord {
        std::string Event;
        std::string From;
        std::vector<char> Payload;
        MSGPACK_DEFINE(Event,From,Payload);
    };

    inline bool operator==(QueryRecord const &lhs, QueryRecord const &rhs)
    {
        return (lhs.Event == rhs.Event &&
                lhs.From == rhs.From &&
                lhs.Payload == rhs.Payload);
    }

    std::ostream &operator<<(std::ostream &os, const QueryRecord &m);
    
    // --------------------------
    struct RecordBase {
        std::string Event;
        MSGPACK_DEFINE(Event);
    };

    // --------------------------
    struct Agent {
        std::string name;
        MSGPACK_DEFINE(name);
    };

    inline bool operator==(Agent const &lhs, Agent const &rhs)
    {
        return (lhs.name == rhs.name);
    }

    struct Runtime {
        std::string os;
        std::string arch;
        std::string version;
        std::string max_procs;
        std::string goroutines;
        std::string cpu_count;
        MSGPACK_DEFINE(os, arch, version, max_procs, goroutines, cpu_count);
    };
    inline bool operator==(Runtime const &lhs, Runtime const &rhs)
    {
        return (lhs.os == rhs.os &&
                lhs.arch == rhs.arch &&
                lhs.version == rhs.version &&
                lhs.max_procs == rhs.max_procs &&
                lhs.goroutines == rhs.goroutines &&
                lhs.cpu_count == rhs.cpu_count);
    }

    struct Serf {
        std::string failed;
        std::string left;
        std::string event_time;
        std::string query_time;
        std::string event_queue;
        std::string members;
        std::string member_time;
        std::string intent_queue;
        std::string query_queue;
        MSGPACK_DEFINE(failed, left, event_time, query_time, event_queue, members, member_time, intent_queue, query_queue);
    };

    inline bool operator==(Serf const &lhs, Serf const &rhs)
    {
        return (lhs.failed == rhs.failed &&
                lhs.left == rhs.left &&
                lhs.event_time == rhs.event_time &&
                lhs.query_time == rhs.query_time &&
                lhs.event_queue == rhs.event_queue &&
                lhs.members == rhs.members &&
                lhs.member_time == rhs.member_time &&
                lhs.intent_queue == rhs.intent_queue &&
                lhs.query_queue == rhs.query_queue);
    }
    
    struct StatsResponse {
        Agent agent;
        Runtime runtime;
        Serf serf;
        MSGPACK_DEFINE(agent,runtime,serf);
    };

    inline bool operator==(StatsResponse const &lhs, StatsResponse const &rhs)
    {
        return (lhs.agent == rhs.agent &&
                lhs.runtime == rhs.runtime &&
                lhs.serf == rhs.serf);
    }

    std::ostream &operator<<(std::ostream &os, const StatsResponse &r);    

    void printMsgPack(std::string const& buf);

};
