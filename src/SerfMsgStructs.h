#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>
#define MSGPACK_USE_DEFINE_MAP
#include <msgpack.hpp>


struct RequestHeader {
    std::string Command;
    unsigned long Seq;
    MSGPACK_DEFINE(Command,Seq);
};
inline bool operator==(RequestHeader const& lhs, RequestHeader const& rhs) {
    return ((lhs.Command == rhs.Command) && (lhs.Seq == rhs.Seq));
}

struct ResponseHeader {
    unsigned long Seq;
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
    std::string Payload;
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
    std::vector<int> Addr;
    int Port;
    std::map<std::string,std::string> Tags;
    std::string Status;
    int ProtocolMin;
    int ProtocolMax;
    int ProtocolCur;
    int DelegateMin;
    int DelegateMax;
    int DelegateCur;
    MSGPACK_DEFINE(Name,Addr,Port,Tags,Status,
                   ProtocolMin,ProtocolMax,ProtocolCur,
                   DelegateMin,DelegateMax,DelegateCur);
};

struct MembersResponse {
    std::vector<Member> Members;
    MSGPACK_DEFINE(Members);
};

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

struct LogEntry {
    std::string Log;
    MSGPACK_DEFINE(Log);
};

struct StopRequest {
    int Stop;
    MSGPACK_DEFINE(Stop);
};

void printMsgPack(std::string const& buf);
