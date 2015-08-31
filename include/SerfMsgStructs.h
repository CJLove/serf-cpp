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

struct RequestHeader {
    std::string Command;
    unsigned long Seq;
    MSGPACK_DEFINE(Command,Seq);
};
inline bool operator==(RequestHeader const& lhs, RequestHeader const& rhs) {
    return ((lhs.Command == rhs.Command) && (lhs.Seq == rhs.Seq));
}

struct ResponseHeader {
    unsigned long long Seq;
    std::string Error;
    MSGPACK_DEFINE(Seq,Error);
};
inline bool operator==(ResponseHeader const& lhs, ResponseHeader const& rhs) {
    return ((lhs.Seq == rhs.Seq) && (lhs.Error == rhs.Error));
}

struct HandshakeRequest {
    int Version;
    MSGPACK_DEFINE(Version);
};
inline bool operator==(HandshakeRequest const& lhs, HandshakeRequest const& rhs) {
    return (lhs.Version == rhs.Version);
}

struct AuthRequest {
    std::string AuthKey;
    MSGPACK_DEFINE(AuthKey);
};
inline bool operator==(AuthRequest const &lhs, AuthRequest const& rhs) {
    return (lhs.AuthKey == rhs.AuthKey);
}

struct EventRequest {
    std::string Name;
    std::vector<char> Payload;
    bool Coalesce;
    MSGPACK_DEFINE(Name,Payload,Coalesce);
};
inline bool operator==(EventRequest const& lhs, EventRequest const& rhs) {
    return ((lhs.Name == rhs.Name) && (lhs.Payload == rhs.Payload) && (lhs.Coalesce == rhs.Coalesce));
}

struct ForceLeaveRequest {
    std::string Node;
    MSGPACK_DEFINE(Node);
};
inline bool operator==(ForceLeaveRequest const& lhs, ForceLeaveRequest const& rhs) {
    return (lhs.Node == rhs.Node);
}

struct JoinRequest {
    std::vector<std::string> Existing;
    bool Replay;
    MSGPACK_DEFINE(Existing,Replay);
};
inline bool operator==(JoinRequest const& lhs, JoinRequest const& rhs) {
    return ((lhs.Existing == rhs.Existing) && (lhs.Replay == rhs.Replay));
}
    
struct JoinResponse {
    int Num;
    MSGPACK_DEFINE(Num);
};
inline bool operator==(JoinResponse const& lhs, JoinResponse const& rhs) {
    return (lhs.Num == rhs.Num);
}

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
             

struct MembersResponse {
    std::vector<Member> Members;
    MSGPACK_DEFINE(Members);
};
inline bool operator==(MembersResponse const &lhs, MembersResponse const& rhs) {
    return (lhs.Members == rhs.Members);
}
std::ostream &operator<<(std::ostream &os, const MembersResponse &m);

 
struct MembersFilteredRequest {
    std::map<std::string,std::string> Tags;
    std::string Status;
    std::string Name;
    MSGPACK_DEFINE(Tags,Status,Name);
};

struct TagsRequest {
    std::map<std::string,std::string> Tags;
    std::vector<std::string> DeleteTags;
    MSGPACK_DEFINE(Tags,DeleteTags);
};

struct StreamRequest {
    std::string Type;
    MSGPACK_DEFINE(Type);
};

struct MonitorRequest {
    std::string LogLevel;
    MSGPACK_DEFINE(LogLevel);
};

struct StopRequest {
    unsigned long long Stop;
    MSGPACK_DEFINE(Stop);
};
    

struct LogRecord {
    std::string Log;
    MSGPACK_DEFINE(Log);
};

struct UserEventRecord {
    std::string Event;
    unsigned long long LTime;
    std::string Name;
    std::vector<char> Payload;
    bool Coalesce;
    MSGPACK_DEFINE(Event,LTime,Name,Payload,Coalesce);
};

struct MemberEventRecord {
    std::string Event;
    std::vector<Member> Members;
    MSGPACK_DEFINE(Event,Members);
};

struct QueryRecord {
    std::string Type;
    std::string From;
    std::vector<char> Payload;
};

void printMsgPack(std::string const& buf);

};
