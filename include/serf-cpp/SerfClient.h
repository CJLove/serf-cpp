#pragma once
#include "SerfMsgStructs.h"

namespace SerfCpp {

class ISerfLogListener;
class ISerfEventListener;
class ISerfQueryListener;

class SerfClient {
public:
    enum SerfResponse { SUCCESS, FAILURE, TIMEOUT };

    // Default ctor - configured to connect to localhost and default port
    SerfClient();

    // Dtor
    ~SerfClient();

    SerfClient(const SerfClient &) = delete;
    SerfClient(const SerfClient &&) = delete;

    void operator=(const SerfClient&) = delete;
    void operator=(const SerfClient&&) = delete;
    
    // Connect to serf agent
    SerfResponse Connect(const std::string &ipAddr = "127.0.0.1", const int16_t &port = 7373);

    // Is the client connected to an agent?
    bool IsConnected();

    // Close the agent connection
    SerfResponse Close();

    //
    // Serf Client RPC methods
    //
    SerfResponse Join(SerfStringArray &addrs, bool replay, int &nodeCount);

    SerfResponse Auth(std::string &authKey);

    SerfResponse InstallKey(std::string &key, KeyResponse &keys);

    SerfResponse UseKey(std::string &key);

    SerfResponse RemoveKey(std::string &key, KeyResponse &keys);

    SerfResponse ListKeys(KeyListResponse &keys);

    SerfResponse Members(MembersResponse &members);

    SerfResponse MembersFiltered(const SerfStringMap &tags, const std::string &status, const std::string &name,
                                 MembersResponse &members);

    SerfResponse Event(const std::string &name, const SerfPayload &payload, bool coalesce);

    SerfResponse ForceLeave(const std::string &nodeName);

    SerfResponse Tags(const SerfStringMap &tags, const SerfStringArray &deleteTags);

    SerfResponse Leave();

    SerfResponse Monitor(const std::string &level, ISerfLogListener *listener, uint64_t
     &seq);

    SerfResponse Stream(const std::string &type, ISerfEventListener *listener, uint64_t
     &seq);

    SerfResponse Stop(const uint64_t
     &seq);

    SerfResponse Stats(StatsResponse &stats);

    SerfResponse Respond(const uint64_t
     &id, const SerfPayload &payload);

    SerfResponse GetCoordinate(const std::string &node, CoordResponse &coordinate);

    SerfResponse Query(const std::string &name, const SerfPayload &payload, ISerfQueryListener *listener,
                       // Following args are optional
                       bool requestAck = false, uint64_t
                        timeout = 15000000000ULL,
                       SerfStringArray *filterNodes = nullptr, SerfStringMap *filterTags = nullptr);

private:
    struct SerfClientImpl;
    SerfClientImpl *m_pImpl;
};

std::ostream &operator<<(std::ostream &os, const SerfClient::SerfResponse &r);

}  // namespace SerfCpp
