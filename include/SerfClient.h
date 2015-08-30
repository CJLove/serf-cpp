#pragma once
#include "SerfMsgStructs.h"

namespace SerfCpp {

class SerfClient {
public:
    // Default ctor - configured to connect to localhost and default port
    SerfClient();

    // Dtor
    ~SerfClient();

    // Connect to serf agent
    bool Connect(const std::string &ipAddr = "127.0.0.1",
                 const short &port = 7373);

    // Is the client connected to an agent?
    bool IsConnected();

    // Close the agent connection
    bool Close();

    //
    // Serf Client RPC methods
    //
    int Join(std::vector<std::string> &addrs,
             bool replay);

    MembersResponse Members();

    MembersResponse MembersFiltered(const std::map<std::string,std::string> & tags,
                                    const std::string &status,
                                    const std::string &name);

    bool Event(const std::string &name, const std::vector<char> &payload, bool coalesce);

    bool ForceLeave(const std::string &nodeName);

    bool Tags(const std::map<std::string,std::string> &tags,
              const std::vector<std::string> &deleteTags);

	bool Leave();    
    

private:
    struct SerfClientImpl;
    SerfClientImpl *m_pImpl;
};

} 	// namespace SerfCpp
