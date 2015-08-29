#pragma once
#include "SerfMsgStructs.h"

namespace SerfCpp {

class SerfClient {
public:
    // Default ctor - configured to connect to localhost and default port
    SerfClient();

    // Ctor - configured to connect to specified IP addr and port
    SerfClient(const std::string&ip, const short &port);

    // Dtor
    ~SerfClient();

    // Connect to serf agent
    bool Connect();

    // Is the client connected to an agent?
    bool IsConnected();

    // Close the agent connection
    bool Close();

    //
    // Serf Client RPC methods
    //
    int Join(std::vector<std::string> &addrs,
             bool replay);

    

private:
    struct SerfClientImpl;
    SerfClientImpl *m_pImpl;
};

} 	// namespace SerfCpp
