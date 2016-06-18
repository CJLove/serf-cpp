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

        // Connect to serf agent
        SerfResponse Connect(const std::string &ipAddr = "127.0.0.1",
                             const short &port = 7373);

        // Is the client connected to an agent?
        bool IsConnected();

        // Close the agent connection
        SerfResponse Close();

        //
        // Serf Client RPC methods
        //
        SerfResponse Join(SerfStringArray &addrs,
                          bool replay, int &nodeCount);

        SerfResponse Auth(std::string &authKey);

        SerfResponse InstallKey(std::string &key, KeyResponse &keys);

        SerfResponse UseKey(std::string &key);

        SerfResponse RemoveKey(std::string &key, KeyResponse &keys);

        SerfResponse ListKeys(KeyListResponse &keys);

        SerfResponse Members(MembersResponse &members);

        SerfResponse MembersFiltered(const SerfStringMap & tags,
                                     const std::string &status,
                                     const std::string &name,
                                     MembersResponse &members);

        SerfResponse Event(const std::string &name, const SerfPayload &payload, bool coalesce);

        SerfResponse ForceLeave(const std::string &nodeName);

        SerfResponse Tags(const SerfStringMap &tags,
                          const SerfStringArray &deleteTags);

        SerfResponse Leave();

        SerfResponse Monitor(const std::string &level, ISerfLogListener *listener, unsigned long long &seq);

        SerfResponse Stream(const std::string &type, ISerfEventListener *listener, unsigned long long &seq);

        SerfResponse Stop(const unsigned long long &seq);

        SerfResponse Stats(StatsResponse &stats);

        SerfResponse Respond(const unsigned long long &id, const SerfPayload &payload);

        SerfResponse GetCoordinate(std::string node, CoordResponse &coordinate);

        SerfResponse Query(const std::string &name, const SerfPayload &payload,
                           ISerfQueryListener *listener,                           
                           // Following args are optional
                           bool requestAck = false, unsigned long long timeout = 15000000000ULL,
                           SerfStringArray *filterNodes = NULL,
                           SerfStringMap *filterTags = NULL);

    private:
        struct SerfClientImpl;
        SerfClientImpl *m_pImpl;
    };

    std::ostream &operator<<(std::ostream &os, const SerfClient::SerfResponse &r); 

} 	// namespace SerfCpp