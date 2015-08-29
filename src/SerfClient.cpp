
#include "SerfClient.h"

namespace SerfCpp {
    //
    // Private Implementation
    //
    struct SerfClient::SerfClientImpl {



        const SerfClient &m_parent;

        SerfClientImpl(const SerfClient& parent): m_parent(parent)
        {
        }

        bool Connect();        
    };


    //
    // Public Implementation
    //
    SerfClient::SerfClient(): m_pImpl(new SerfClientImpl(*this))
    {
    }

    SerfClient::SerfClient(const std::string& ipAddr,
                      const short &port): m_pImpl(new SerfClientImpl(*this))
    {
    }

    SerfClient::~SerfClient()
    {
    }
    
} // namespace SerfCpp
