#pragma once

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#define MSGPACK_USE_DEFINE_MAP
#include <msgpack.hpp>

//
// Structs used with msgpack-c for messages exchanged with serf agent.
//
// For reference, see https://www.serfdom.io/docs/agent/rpc.html for the
// corresponding JSON representation and
// https://github.com/hashicorp/serf/blob/master/client/const.go for the
// reference implementation in golang (helpful in discerning types)
//

namespace SerfCpp {

const int SerfApiVersion = 1;

// Convenience types used in Serf RPC messages
using SerfPayload = std::vector<char>;
using SerfStringArray = std::vector<std::string>;
using SerfStringMap = std::map<std::string, std::string>;
using SerfStringIntMap = std::map<std::string, int>;

// --------------------------
struct RequestHeader {
    std::string Command;
    uint64_t Seq = 0;
    MSGPACK_DEFINE(Command, Seq)
};
inline bool operator==(RequestHeader const &lhs, RequestHeader const &rhs) {
    return ((lhs.Command == rhs.Command) && (lhs.Seq == rhs.Seq));
}

// --------------------------
struct ResponseHeader {
    uint64_t Seq = 0;
    std::string Error;
    MSGPACK_DEFINE(Seq, Error)
};
inline bool operator==(ResponseHeader const &lhs, ResponseHeader const &rhs) {
    return ((lhs.Seq == rhs.Seq) && (lhs.Error == rhs.Error));
}

std::ostream &operator<<(std::ostream &os, const ResponseHeader &r);

// --------------------------
struct HandshakeRequest {
    int Version = 0;
    MSGPACK_DEFINE(Version)
};
inline bool operator==(HandshakeRequest const &lhs, HandshakeRequest const &rhs) { return (lhs.Version == rhs.Version); }

// --------------------------
struct AuthRequest {
    std::string AuthKey;
    MSGPACK_DEFINE(AuthKey)
};
inline bool operator==(AuthRequest const &lhs, AuthRequest const &rhs) { return (lhs.AuthKey == rhs.AuthKey); }

std::ostream &operator<<(std::ostream &os, const AuthRequest &r);
// --------------------------
struct CoordRequest {
    std::string Node;
    MSGPACK_DEFINE(Node)
};
inline bool operator==(CoordRequest const &lhs, CoordRequest const &rhs) { return (lhs.Node == rhs.Node); }

std::ostream &operator<<(std::ostream &os, const CoordRequest &r);

struct Coordinate {
    std::vector<double> Vec;
    double Error = 0.0;
    double Adjustment = 0.0;
    double Height = 0.0;

    MSGPACK_DEFINE(Adjustment, Error, Height, Vec)
    //        MSGPACK_DEFINE(Vec,Error,Adjustment,Height);
};
inline bool operator==(Coordinate const &lhs, Coordinate const &rhs) {
    return (lhs.Adjustment == rhs.Adjustment && lhs.Error == rhs.Error && lhs.Height == rhs.Height && lhs.Vec == rhs.Vec);
}
std::ostream &operator<<(std::ostream &os, const Coordinate &r);

struct CoordResponse {
    Coordinate Coord;
    bool Ok = false;
    MSGPACK_DEFINE(Coord, Ok)
};
inline bool operator==(CoordResponse const &lhs, CoordResponse const &rhs) {
    return (lhs.Coord == rhs.Coord && lhs.Ok == rhs.Ok);
}
std::ostream &operator<<(std::ostream &os, const CoordResponse &r);

// --------------------------
// Used for install-key, use-key, and remove-key requests
struct KeyRequest {
    std::string Key;
    MSGPACK_DEFINE(Key)
};
inline bool operator==(KeyRequest const &lhs, KeyRequest const &rhs) { return (lhs.Key == rhs.Key); }

std::ostream &operator<<(std::ostream &os, const KeyRequest &r);

// --------------------------
struct RespondRequest {
    uint64_t ID = 0;
    SerfPayload Payload;
    MSGPACK_DEFINE(ID, Payload)
};
inline bool operator==(RespondRequest const &lhs, RespondRequest const &rhs) {
    return (lhs.ID == rhs.ID && lhs.Payload == rhs.Payload);
}
std::ostream &operator<<(std::ostream &os, const RespondRequest &r);

// --------------------------
// Used for install-key and remove-key responses
struct KeyResponse {
    SerfStringMap Messages;
    int NumErr = 0;
    int NumNodes = 0;
    int NumResp = 0;
    MSGPACK_DEFINE(Messages, NumErr, NumNodes, NumResp)
};
inline bool operator==(KeyResponse const &lhs, KeyResponse const &rhs) {
    return (lhs.Messages == rhs.Messages && lhs.NumErr == rhs.NumErr && lhs.NumNodes == rhs.NumNodes &&
            lhs.NumResp == rhs.NumResp);
}
std::ostream &operator<<(std::ostream &os, const KeyResponse &r);

// --------------------------
struct KeyListResponse {
    SerfStringMap Messages;
    SerfStringIntMap Keys;
    int NumErr = 0;
    int NumNodes = 0;
    int NumResp = 0;
    MSGPACK_DEFINE(Messages, Keys, NumErr, NumNodes, NumResp)
};
inline bool operator==(KeyListResponse const &lhs, KeyListResponse const &rhs) {
    return (lhs.Messages == rhs.Messages && lhs.Keys == rhs.Keys && lhs.NumErr == rhs.NumErr && lhs.NumNodes == rhs.NumNodes &&
            lhs.NumResp == rhs.NumResp);
}
std::ostream &operator<<(std::ostream &os, const KeyListResponse &r);

// --------------------------
struct EventRequest {
    std::string Name;
    SerfPayload Payload;
    bool Coalesce = false;
    MSGPACK_DEFINE(Name, Payload, Coalesce)
};
inline bool operator==(EventRequest const &lhs, EventRequest const &rhs) {
    return ((lhs.Name == rhs.Name) && (lhs.Payload == rhs.Payload) && (lhs.Coalesce == rhs.Coalesce));
}

// --------------------------
struct QueryRequest {
    SerfStringArray FilterNodes;
    SerfStringMap FilterTags;
    bool RequestAck = false;
    uint64_t Timeout = 0;
    std::string Name;
    SerfPayload Payload;
    MSGPACK_DEFINE(FilterNodes, FilterTags, RequestAck, Timeout, Name, Payload)
};
inline bool operator==(QueryRequest const &lhs, QueryRequest const &rhs) {
    return ((lhs.FilterNodes == rhs.FilterNodes) && (lhs.FilterTags == rhs.FilterTags) && (lhs.RequestAck == rhs.RequestAck) &&
            (lhs.Timeout == rhs.Timeout) && (lhs.Name == rhs.Name) && (lhs.Payload == rhs.Payload));
}

// --------------------------
struct ForceLeaveRequest {
    std::string Node;
    MSGPACK_DEFINE(Node)
};
inline bool operator==(ForceLeaveRequest const &lhs, ForceLeaveRequest const &rhs) { return (lhs.Node == rhs.Node); }

// --------------------------
struct JoinRequest {
    SerfStringArray Existing;
    bool Replay = false;
    MSGPACK_DEFINE(Existing, Replay)
};
inline bool operator==(JoinRequest const &lhs, JoinRequest const &rhs) {
    return ((lhs.Existing == rhs.Existing) && (lhs.Replay == rhs.Replay));
}

// --------------------------
struct JoinResponse {
    int Num = 0;
    MSGPACK_DEFINE(Num)
};
inline bool operator==(JoinResponse const &lhs, JoinResponse const &rhs) { return (lhs.Num == rhs.Num); }

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
    int Port = 0;
    SerfStringMap Tags;
    std::string Status;
    unsigned char ProtocolMin = 0;
    unsigned char ProtocolMax = 0;
    unsigned char ProtocolCur = 0;
    unsigned char DelegateMin = 0;
    unsigned char DelegateMax = 0;
    unsigned char DelegateCur = 0;

    MSGPACK_DEFINE(Name, Addr, Port, Tags, Status, ProtocolMin, ProtocolMax, ProtocolCur, DelegateMin, DelegateMax, DelegateCur)
};
inline bool operator==(Member const &lhs, Member const &rhs) {
    return ((lhs.Name == rhs.Name) && (lhs.Addr == rhs.Addr) && (lhs.Port == rhs.Port) && (lhs.Tags == rhs.Tags) &&
            (lhs.Status == rhs.Status) && (lhs.ProtocolMin == rhs.ProtocolMin) && (lhs.ProtocolMax == rhs.ProtocolMax) &&
            (lhs.ProtocolCur == rhs.ProtocolCur) && (lhs.DelegateMin == rhs.DelegateMin) &&
            (lhs.DelegateMax == rhs.DelegateMax) && (lhs.DelegateMin == rhs.DelegateMin));
}

std::ostream &operator<<(std::ostream &os, const Member &r);

// --------------------------
struct MembersResponse {
    std::vector<Member> Members;
    MSGPACK_DEFINE(Members)
};
inline bool operator==(MembersResponse const &lhs, MembersResponse const &rhs) { return (lhs.Members == rhs.Members); }
std::ostream &operator<<(std::ostream &os, const MembersResponse &r);

// --------------------------
struct MembersFilteredRequest {
    SerfStringMap Tags;
    std::string Status;
    std::string Name;
    MSGPACK_DEFINE(Tags, Status, Name)
};
inline bool operator==(MembersFilteredRequest const &lhs, MembersFilteredRequest const &rhs) {
    return (lhs.Tags == rhs.Tags && lhs.Status == rhs.Status && lhs.Name == rhs.Name);
}

std::ostream &operator<<(std::ostream &os, const MembersFilteredRequest &r);

// --------------------------
struct TagsRequest {
    SerfStringMap Tags;
    SerfStringArray DeleteTags;
    MSGPACK_DEFINE(Tags, DeleteTags)
};
inline bool operator==(TagsRequest const &lhs, TagsRequest const &rhs) {
    return (lhs.Tags == rhs.Tags && lhs.DeleteTags == rhs.DeleteTags);
}

std::ostream &operator<<(std::ostream &os, const TagsRequest &r);

// --------------------------
struct StreamRequest {
    std::string Type;
    MSGPACK_DEFINE(Type)
};

inline bool operator==(StreamRequest const &lhs, StreamRequest const &rhs) { return (lhs.Type == rhs.Type); }

std::ostream &operator<<(std::ostream &os, const StreamRequest &r);

// --------------------------
struct MonitorRequest {
    std::string LogLevel;
    MSGPACK_DEFINE(LogLevel)
};

inline bool operator==(MonitorRequest const &lhs, MonitorRequest const &rhs) { return (lhs.LogLevel == rhs.LogLevel); }

std::ostream &operator<<(std::ostream &os, const MonitorRequest &r);

// --------------------------
struct StopRequest {
    uint64_t Stop = 0;
    MSGPACK_DEFINE(Stop)
};

inline bool operator==(StopRequest const &lhs, StopRequest const &rhs) { return (lhs.Stop == rhs.Stop); }

std::ostream &operator<<(std::ostream &os, const StopRequest &r);

// --------------------------
struct LogRecord {
    std::string Log;
    MSGPACK_DEFINE(Log)
};

inline bool operator==(LogRecord const &lhs, LogRecord const &rhs) { return (lhs.Log == rhs.Log); }

std::ostream &operator<<(std::ostream &os, const LogRecord &r);

// --------------------------
struct UserEventRecord {
    std::string Event;
    uint64_t LTime = 0;
    std::string Name;
    SerfPayload Payload;
    bool Coalesce = false;
    MSGPACK_DEFINE(Event, LTime, Name, Payload, Coalesce)
};

inline bool operator==(UserEventRecord const &lhs, UserEventRecord const &rhs) {
    return (lhs.Event == rhs.Event && lhs.LTime == rhs.LTime && lhs.Name == rhs.Name && lhs.Payload == rhs.Payload &&
            lhs.Coalesce == rhs.Coalesce);
}

std::ostream &operator<<(std::ostream &os, const UserEventRecord &r);

// --------------------------
struct MemberEventRecord {
    std::string Event;
    std::vector<Member> Members;
    MSGPACK_DEFINE(Event, Members)
};

inline bool operator==(MemberEventRecord const &lhs, MemberEventRecord const &rhs) {
    return (lhs.Event == rhs.Event && lhs.Members == rhs.Members);
}

std::ostream &operator<<(std::ostream &os, const MemberEventRecord &r);

// --------------------------
struct QueryRecord {
    std::string Event;
    uint64_t ID = 0;
    std::string Name;
    SerfPayload Payload;
    uint64_t LTime = 0;
    MSGPACK_DEFINE(Event, ID, Name, Payload, LTime)
};

inline bool operator==(QueryRecord const &lhs, QueryRecord const &rhs) {
    return (lhs.Event == rhs.Event && lhs.ID == rhs.ID && lhs.Name == rhs.Name && lhs.LTime == rhs.LTime &&
            lhs.Payload == rhs.Payload);
}

std::ostream &operator<<(std::ostream &os, const QueryRecord &r);

// --------------------------
// Response to query from individual node
struct NodeResponseBase {
    std::string Type;
    MSGPACK_DEFINE(Type)
};

inline bool operator==(NodeResponseBase const &lhs, NodeResponseBase const &rhs) { return (lhs.Type == rhs.Type); }

std::ostream &operator<<(std::ostream &os, const NodeResponseBase &r);

struct NodeAck {
    std::string Type;
    std::string From;
    MSGPACK_DEFINE(Type, From)
};

inline bool operator==(NodeAck const &lhs, NodeAck const &rhs) { return (lhs.Type == rhs.Type && lhs.From == rhs.From); }

std::ostream &operator<<(std::ostream &os, const NodeAck &r);

struct NodeResponse {
    std::string Type;
    std::string From;
    SerfPayload Payload;
    MSGPACK_DEFINE(Type, From, Payload)
};

inline bool operator==(NodeResponse const &lhs, NodeResponse const &rhs) {
    return (lhs.Type == rhs.Type && lhs.From == rhs.From && lhs.Payload == rhs.Payload);
}

std::ostream &operator<<(std::ostream &os, const NodeResponse &r);

// --------------------------
struct RecordBase {
    std::string Event;
    MSGPACK_DEFINE(Event)
};

// --------------------------
struct Agent {
    std::string name;
    MSGPACK_DEFINE(name)
};

inline bool operator==(Agent const &lhs, Agent const &rhs) { return (lhs.name == rhs.name); }

struct Runtime {
    std::string os;
    std::string arch;
    std::string version;
    std::string max_procs;
    std::string goroutines;
    std::string cpu_count;
    MSGPACK_DEFINE(os, arch, version, max_procs, goroutines, cpu_count)
};
inline bool operator==(Runtime const &lhs, Runtime const &rhs) {
    return (lhs.os == rhs.os && lhs.arch == rhs.arch && lhs.version == rhs.version && lhs.max_procs == rhs.max_procs &&
            lhs.goroutines == rhs.goroutines && lhs.cpu_count == rhs.cpu_count);
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
    MSGPACK_DEFINE(failed, left, event_time, query_time, event_queue, members, member_time, intent_queue, query_queue)
};

inline bool operator==(Serf const &lhs, Serf const &rhs) {
    return (lhs.failed == rhs.failed && lhs.left == rhs.left && lhs.event_time == rhs.event_time &&
            lhs.query_time == rhs.query_time && lhs.event_queue == rhs.event_queue && lhs.members == rhs.members &&
            lhs.member_time == rhs.member_time && lhs.intent_queue == rhs.intent_queue && lhs.query_queue == rhs.query_queue);
}

struct StatsResponse {
    Agent agent;
    Runtime runtime;
    Serf serf;
    MSGPACK_DEFINE(agent, runtime, serf)
};

inline bool operator==(StatsResponse const &lhs, StatsResponse const &rhs) {
    return (lhs.agent == rhs.agent && lhs.runtime == rhs.runtime && lhs.serf == rhs.serf);
}

std::ostream &operator<<(std::ostream &os, const StatsResponse &r);

void printMsgPack(std::string const &buf);

}  // namespace SerfCpp
