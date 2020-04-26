#pragma once
#include "SerfMsgStructs.h"
#include <memory>

namespace SerfCpp {

class ISerfLogListener;
class ISerfEventListener;
class ISerfQueryListener;

constexpr int SERF_CPP_VERSION_MAJOR=1;
constexpr int SERF_CPP_VERSION_MINOR=1;
constexpr int SERF_CPP_VERSION_PATCH=0;

static const char* SERF_CPP_LOCALHOST="127.0.0.1";
constexpr uint16_t SERF_PORT=7373;
constexpr uint64_t SERF_CPP_DEFAULT_TIMEOUT=15000000000ULL;

/**
 * @brief The SerfClient class establishes an RPC connection to a serf agent in support
 *        of sending other RPC commands.
 *
 */
class SerfClient {
public:
    /**
     * @brief Results of SerfClient request methods
     *
     */
    enum SerfResponse { SUCCESS, FAILURE, TIMEOUT };

    /**
     * @brief Construct a new Serf Client object, defaulting to connect to localhost
     *
     */
    SerfClient();

    /**
     * @brief Destroy the Serf Client object
     *
     */
    ~SerfClient();

    /**
     * @brief Copy and move constructors disabled
     *
     */
    SerfClient(const SerfClient &) = delete;
    SerfClient(const SerfClient &&) = delete;

    /**
     * @brief Assignment and move assignment disabled
     *
     */
    void operator=(const SerfClient &) = delete;
    void operator=(const SerfClient &&) = delete;

    /**
     * @brief Connect to a serf agent at the specified IP address and port number
     *
     * @param ipAddr
     * @param port
     * @return SerfResponse
     */
    SerfResponse Connect(const std::string &ipAddr = SERF_CPP_LOCALHOST, const uint16_t &port = SERF_PORT);

    /**
     * @brief Return whether there is a connection established to a serf agent
     *
     * @return true/false
     */
    bool IsConnected();

    /**
     * @brief Close the connection to the serf agent
     *
     * @return SerfResponse
     */
    SerfResponse Close();

    //
    // Serf Client RPC methods
    //

    /**
     * @brief Join this agent to one or more agents, returning the number of nodes joined
     *
     * @param addrs - vector of one or more IP addresses
     * @param replay - replay prior user events (true/false)
     * @param nodeCount - return number of nodes joined
     * @return SerfResponse
     */
    SerfResponse Join(const SerfStringArray &addrs, bool replay, int &nodeCount);

    /**
     * @brief Authentication command to send after a connection is established
     *
     * @param authKey - authentication token
     * @return SerfResponse
     */
    SerfResponse Auth(const std::string &authKey);

    /**
     * @brief Install a new authentication key
     *
     * @param key - key to add
     * @param keys - response after key is distributed to all nodes
     * @return SerfResponse
     */
    SerfResponse InstallKey(const std::string &key, KeyResponse &keys);

    /**
     * @brief Use a specific key from the key-ring for encrypting messages
     *
     * @param key
     * @return SerfResponse
     */
    SerfResponse UseKey(const std::string &key);

    /**
     * @brief Remove a specific key from the key-ring
     *
     * @param key - key to remove
     * @param keys - response after key is removed from all nodes
     * @return SerfResponse
     */
    SerfResponse RemoveKey(const std::string &key, KeyResponse &keys);

    /**
     * @brief List keys from the key-ring
     *
     * @param keys - information on all keys in the key-ring, including how many nodes know about each key
     * @return SerfResponse
     */
    SerfResponse ListKeys(KeyListResponse &keys);

    /**
     * @brief List info on each member of the cluster
     *
     * @param members - cluster membership info
     * @return SerfResponse
     */
    SerfResponse Members(MembersResponse &members);

    /**
     * @brief List info on each member of the cluster, subject to filtering
     *
     * @param tags - tags to use for filtering
     * @param status - status to use for filtering
     * @param name - name to use for filtering
     * @param members - cluster membership info after applying filters
     * @return SerfResponse
     */
    SerfResponse MembersFiltered(const SerfStringMap &tags, const std::string &status, const std::string &name,
                                 MembersResponse &members);

    /**
     * @brief Send a user event
     *
     * @param name - event name
     * @param payload - event data (raw bytes)
     * @param coalesce - whether to coalesce events
     * @return SerfResponse
     */
    SerfResponse Event(const std::string &name, const SerfPayload &payload, bool coalesce);

    /**
     * @brief Force a node to leave the cluster
     *
     * @param nodeName - node which should leave
     * @return SerfResponse
     */
    SerfResponse ForceLeave(const std::string &nodeName);

    /**
     * @brief Add/modify/remove tags on a serf agent node
     *
     * @param tags - tags to add/modify
     * @param deleteTags - tags to remove
     * @return SerfResponse
     */
    SerfResponse Tags(const SerfStringMap &tags, const SerfStringArray &deleteTags);

    /**
     * @brief Trigger the agent gracefully exit the cluster
     *
     * @return SerfResponse
     */
    SerfResponse Leave();

    /**
     * @brief Register for log messages from the serf agent
     *
     * @param level - level of logging
     * @param listener - callback method for each log message
     * @param seq - sequence number to use in subsequent Stop() request
     * @return SerfResponse
     */
    SerfResponse Monitor(const std::string &level, ISerfLogListener *listener, uint64_t &seq);

    /**
     * @brief Register for events from the serf agent
     *
     * @param type - types of events to receive
     * @param listener - callback method for each event
     * @param seq - sequence number to use in subsequent Stop() request
     * @return SerfResponse
     */
    SerfResponse Stream(const std::string &type, ISerfEventListener *listener, uint64_t &seq);

    /**
     * @brief Stop receiving events or log messages
     *
     * @param seq - sequence number of the original Stream() or Monitor() request
     * @return SerfResponse
     */
    SerfResponse Stop(const uint64_t &seq);

    /**
     * @brief Return statistics about the running serf agent
     *
     * @param stats - agent statistics
     * @return SerfResponse
     */
    SerfResponse Stats(StatsResponse &stats);

    /**
     * @brief Respond to a query request
     *
     * @param id - sequence number of the query
     * @param payload - resposne
     * @return SerfResponse
     */
    SerfResponse Respond(const uint64_t &id, const SerfPayload &payload);

    /**
     * @brief Get the Coordinate of a serf node in the cluster
     *
     * @param node - node name
     * @param coordinate - coordinate info
     * @return SerfResponse
     */
    SerfResponse GetCoordinate(const std::string &node, CoordResponse &coordinate);

    /**
     * @brief Send a query request to nodes in the cluster
     *
     * @param name
     * @param payload
     * @param listener
     * @param requestAck
     * @param timeout
     * @param filterNodes
     * @param filterTags
     * @return SerfResponse
     */
    SerfResponse Query(const std::string &name, const SerfPayload &payload, ISerfQueryListener *listener,
                       // Following args are optional
                       bool requestAck = false, uint64_t timeout = SERF_CPP_DEFAULT_TIMEOUT, SerfStringArray *filterNodes = nullptr,
                       SerfStringMap *filterTags = nullptr);

    /**
     * @brief Return version information about the serf-cpp library
     *
     * @param major - major version
     * @param minor - minor version
     * @param patch - patch version
     */
    void Version(uint32_t &major, uint32_t &minor, uint32_t &patch);

private:
    struct SerfClientImpl;
    std::unique_ptr<SerfClientImpl> m_pImpl;
};

std::ostream &operator<<(std::ostream &os, const SerfClient::SerfResponse &r);

}  // namespace SerfCpp
