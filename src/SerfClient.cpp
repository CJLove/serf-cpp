#include "SerfClient.h"
#include "SerfIoThread.h"

namespace SerfCpp {
    //
    // Private Implementation
    //
    struct SerfClient::SerfClientImpl {


        SerfIoThread m_serfThread;

        const SerfClient &m_parent;

        SerfClientImpl(const SerfClient& parent): 
            m_parent(parent)
        {
        }

    };


    //
    // Public Implementation
    //
    SerfClient::SerfClient(): m_pImpl(new SerfClientImpl(*this))
    {
    }

    SerfClient::~SerfClient()
    {
    }

    bool SerfClient::Connect(const std::string &ipAddr, const short &port)
    {
        return m_pImpl->m_serfThread.Connect(ipAddr,port);
    }

    bool SerfClient::Close()
    {
        return m_pImpl->m_serfThread.Close();
    }

    int SerfClient::Join(std::vector<std::string> &addrs,
             bool replay)
    {
        RequestHeader hdr;
        hdr.Command = "join";
        JoinRequest join;
        join.Existing = addrs;
        join.Replay = replay;

        // Channel for receiving response
        ResultChannel<JoinResponse> channel;
        
        m_pImpl->m_serfThread.sendData(hdr,join, &channel);

        channel.consume();

        return channel.m_data.Num;
    }

    MembersResponse SerfClient::Members()
    {
        RequestHeader hdr;
        hdr.Command = "members";

        // Channel for receiving response
        ResultChannel<MembersResponse> channel;

        m_pImpl->m_serfThread.sendData(hdr,&channel);

        channel.consume();

        return channel.m_data;
    }

    MembersResponse SerfClient::MembersFiltered(const std::map<std::string,std::string> & tags,
                                                const std::string &status,
                                                const std::string &name)
    {
        RequestHeader hdr;
        hdr.Command = "members-filtered";
        MembersFilteredRequest req;
        req.Tags = tags;
        req.Status = status;
        req.Name = name;;

        // Channel for receiving response
        ResultChannel<MembersResponse> channel;

        m_pImpl->m_serfThread.sendData(hdr,req,&channel);

        channel.consume();

        return channel.m_data;
    }

    bool SerfClient::Event(const std::string &name, const std::vector<char> &payload, bool coalesce)
    {
        RequestHeader hdr;
        EventRequest event;
        hdr.Command = "event";
        event.Name = name;
        event.Payload = payload;
        event.Coalesce = coalesce;
        
        // Channel for receiving response
        ResultChannel<bool> channel;

        m_pImpl->m_serfThread.sendData(hdr,event,&channel);

        channel.consume();

        return (channel.m_hdr.Error == "");
    }

    bool SerfClient::ForceLeave(const std::string &nodeName)
    {
        RequestHeader hdr;
        ForceLeaveRequest req;
        hdr.Command="force-leave";
        req.Node = nodeName;
        
        // Channel for receiving response
        ResultChannel<bool> channel;

        m_pImpl->m_serfThread.sendData(hdr,req,&channel);

        channel.consume();

        return (channel.m_hdr.Error == "");
    }
    
    bool SerfClient::Tags(const std::map<std::string,std::string> &tags,
                          const std::vector<std::string> &deleteTags)
    {
        RequestHeader hdr;
        TagsRequest req;
        hdr.Command = "tags";
        req.Tags = tags;
        req.DeleteTags = deleteTags;

        // Channel for receiving response
        ResultChannel<bool> channel;

        m_pImpl->m_serfThread.sendData(hdr,req,&channel);

        channel.consume();

        return (channel.m_hdr.Error == "");
    }

	bool SerfClient::Leave()
    {
        RequestHeader hdr;
        hdr.Command = "leave";

        // Channel for receiving response
        ResultChannel<bool> channel;

        m_pImpl->m_serfThread.sendData(hdr,&channel);

        channel.consume();

        return (channel.m_hdr.Error == "");
    }
        
    
} // namespace SerfCpp
