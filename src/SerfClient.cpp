#include "SerfClient.h"
#include "SerfChannel.h"
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
    std::ostream &operator<<(std::ostream &os, const SerfClient::SerfResponse &r)
    {
        switch(r) {
        case SerfClient::SUCCESS:
            os << "SUCCESS";
            break;
        case SerfClient::FAILURE:
            os << "FAILURE";
            break;
        case SerfClient::TIMEOUT:
            os << "TIMEOUT";
            break;
        }
        return os;
    }
    
    SerfClient::SerfClient(): m_pImpl(new SerfClientImpl(*this))
    {
    }

    SerfClient::~SerfClient()
    {
    }

    SerfClient::SerfResponse
    SerfClient::Connect(const std::string &ipAddr, const short &port)
    {
        return (m_pImpl->m_serfThread.Connect(ipAddr,port)) ? SerfClient::SUCCESS : SerfClient::FAILURE;
    }

    SerfClient::SerfResponse
    SerfClient::Close()
    {
        return (m_pImpl->m_serfThread.Close()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
    }

    SerfClient::SerfResponse
    SerfClient::Join(std::vector<std::string> &addrs,
                     bool replay, int &nodeCount)
    {
        RequestHeader hdr;
        hdr.Command = "join";
        JoinRequest join;
        join.Existing = addrs;
        join.Replay = replay;

        // Channel for receiving response
        ResultChannel<JoinResponse> channel;
        unsigned long long seq = 0;
        
        if (m_pImpl->m_serfThread.sendData(hdr,join, &channel,seq)) {
            channel.consume();

            if (channel.m_dataPending) {
                nodeCount = channel.m_data.Num;
                return (channel.m_hdr.Error == "") ? SerfClient::SUCCESS: SerfClient::FAILURE;
            } else {
                m_pImpl->m_serfThread.removeChannel(seq);
		        return SerfClient::TIMEOUT;                
            }
        }
        return SerfClient::FAILURE;
    }

    SerfClient::SerfResponse
    SerfClient::Members(MembersResponse &members)
    {
        RequestHeader hdr;
        hdr.Command = "members";

        // Channel for receiving response
        ResultChannel<MembersResponse> channel;
        unsigned long long seq = 0;

        if (m_pImpl->m_serfThread.sendData(hdr,&channel,seq)) {
            channel.consume();

        	if (channel.m_dataPending) {
                members = channel.m_data;
                return( channel.m_hdr.Error == "") ? SerfClient::SUCCESS: SerfClient::FAILURE;
            } else {
                m_pImpl->m_serfThread.removeChannel(seq);
		        return SerfClient::TIMEOUT;                
            }
        }
        return SerfClient::FAILURE;
    }

    SerfClient::SerfResponse
    SerfClient::MembersFiltered(const std::map<std::string,std::string> & tags,
                                const std::string &status,
                                const std::string &name,
                                MembersResponse &members)
    {
        RequestHeader hdr;
        hdr.Command = "members-filtered";
        MembersFilteredRequest req;
        req.Tags = tags;
        req.Status = status;
        req.Name = name;;

        // Channel for receiving response
        ResultChannel<MembersResponse> channel;
        unsigned long long seq = 0;

        if (m_pImpl->m_serfThread.sendData(hdr,req,&channel,seq)) {
            channel.consume();

	        if (channel.m_dataPending) {
                members = channel.m_data;
                return (channel.m_hdr.Error == "") ? SerfClient::SUCCESS: SerfClient::FAILURE;
            } else {
                m_pImpl->m_serfThread.removeChannel(seq);
                return SerfClient::TIMEOUT;
            }
        }
        return SerfClient::FAILURE;
    }

    SerfClient::SerfResponse
    SerfClient::Event(const std::string &name, const std::vector<char> &payload, bool coalesce)
    {
        RequestHeader hdr;
        EventRequest event;
        hdr.Command = "event";
        event.Name = name;
        event.Payload = payload;
        event.Coalesce = coalesce;
        
        // Channel for receiving response
        ResultChannel<bool> channel;
        unsigned long long seq = 0;

        if (m_pImpl->m_serfThread.sendData(hdr,event,&channel,seq)) {
            channel.consume();

	        if (channel.m_dataPending) {
                return (channel.m_hdr.Error == "") ? SerfClient::SUCCESS: SerfClient::FAILURE;
            } else {
                m_pImpl->m_serfThread.removeChannel(seq);
                return SerfClient::TIMEOUT;
            }
        }
        return SerfClient::FAILURE;
    }

    SerfClient::SerfResponse
    SerfClient::ForceLeave(const std::string &nodeName)
    {
        RequestHeader hdr;
        ForceLeaveRequest req;
        hdr.Command="force-leave";
        req.Node = nodeName;
        
        // Channel for receiving response
        ResultChannel<bool> channel;
        unsigned long long seq = 0;

        if (m_pImpl->m_serfThread.sendData(hdr,req,&channel,seq)) {
            channel.consume();

	        if (channel.m_dataPending) {
                return (channel.m_hdr.Error == "") ? SerfClient::SUCCESS: SerfClient::FAILURE;
            } else {
                m_pImpl->m_serfThread.removeChannel(seq);
                return SerfClient::TIMEOUT;
            }
        }
        return SerfClient::FAILURE;
    }
    
    SerfClient::SerfResponse
    SerfClient::Tags(const std::map<std::string,std::string> &tags,
                     const std::vector<std::string> &deleteTags)
    {
        RequestHeader hdr;
        TagsRequest req;
        hdr.Command = "tags";
        req.Tags = tags;
        req.DeleteTags = deleteTags;

        // Channel for receiving response
        ResultChannel<bool> channel;
        unsigned long long seq = 0;

        if (m_pImpl->m_serfThread.sendData(hdr,req,&channel,seq)) {
            channel.consume();

	        if (channel.m_dataPending) {
                return (channel.m_hdr.Error == "") ? SerfClient::SUCCESS: SerfClient::FAILURE;
            } else {
                m_pImpl->m_serfThread.removeChannel(seq);
                return SerfClient::TIMEOUT;
            }
        }
        return SerfClient::FAILURE;
    }

    SerfClient::SerfResponse
    SerfClient::Leave()
    {
        RequestHeader hdr;
        hdr.Command = "leave";

        // Channel for receiving response
        ResultChannel<bool> channel;
        unsigned long long seq = 0;

        if (m_pImpl->m_serfThread.sendData(hdr,&channel,seq)) {
            channel.consume();

	        if (channel.m_dataPending) {
                return (channel.m_hdr.Error == "") ? SerfClient::SUCCESS: SerfClient::FAILURE;
            } else {
                m_pImpl->m_serfThread.removeChannel(seq);
		        return SerfClient::TIMEOUT;                
            }
        }
        return SerfClient::FAILURE;
    }

    SerfClient::SerfResponse
    SerfClient::Monitor(const std::string &level, ISerfLogListener *listener,unsigned long long &seq)
    {
        RequestHeader hdr;
        hdr.Command = "monitor";
        MonitorRequest req;
        req.LogLevel = level;

        // Channel for receiving response
        ResultChannel<bool> channel;

        if (m_pImpl->m_serfThread.sendData(hdr,req,&channel,seq)) {
            channel.consume();

	        if (channel.m_dataPending) {
                SerfClient::SerfResponse resp = (channel.m_hdr.Error == "") ? SerfClient::SUCCESS : SerfClient::FAILURE;

                if (resp == SerfClient::SUCCESS) {
                    m_pImpl->m_serfThread.addLogChannel(channel.m_hdr.Seq,listener);
                    seq = channel.m_hdr.Seq;
                }
                return resp;
            } else {
                m_pImpl->m_serfThread.removeChannel(seq);
                return SerfClient::TIMEOUT;
            }
        }
        return SerfClient::FAILURE;
    }

    SerfClient::SerfResponse
    SerfClient::Stream(const std::string &type, ISerfEventListener *listener, unsigned long long &seq)
    {
        RequestHeader hdr;
        hdr.Command = "stream";
        StreamRequest req;
        req.Type = type;

        // Channel for receiving response
        ResultChannel<bool> channel;

        if (m_pImpl->m_serfThread.sendData(hdr,req,&channel,seq)) {
            channel.consume();

	        if (channel.m_dataPending) {
                SerfClient::SerfResponse resp = (channel.m_hdr.Error == "") ? SerfClient::SUCCESS : SerfClient::FAILURE;

                if (resp == SerfClient::SUCCESS) {
                    m_pImpl->m_serfThread.addEventChannel(channel.m_hdr.Seq,listener);
                    seq = channel.m_hdr.Seq;
                }
                return resp;
            } else {
                m_pImpl->m_serfThread.removeChannel(seq);
                return SerfClient::TIMEOUT;
            }
        }
        return SerfClient::FAILURE;
    }

    SerfClient::SerfResponse
    SerfClient::Stop(const unsigned long long &stopSeq)
    {
        RequestHeader hdr;
        StopRequest req;
        hdr.Command = "stop";
        req.Stop = stopSeq;

        // Channel for receiving response
        ResultChannel<bool> channel;
        unsigned long long seq = 0;

        if (m_pImpl->m_serfThread.sendData(hdr,req,&channel,seq)) {

            channel.consume();

	        if (channel.m_dataPending) {
                SerfClient::SerfResponse resp = (channel.m_hdr.Error == "") ? SerfClient::SUCCESS : SerfClient::FAILURE;

                if (resp == SerfClient::SUCCESS) {
                    m_pImpl->m_serfThread.removeChannel(stopSeq);
                }
                return resp;
            } else {
                m_pImpl->m_serfThread.removeChannel(seq);
                return SerfClient::TIMEOUT;
            }
        }
        return SerfClient::FAILURE;
    }

    SerfClient::SerfResponse
    SerfClient::Stats(StatsResponse &stats)
    {
        RequestHeader hdr;
        TagsRequest req;
        hdr.Command = "stats";

        // Channel for receiving response
        ResultChannel<StatsResponse> channel;
        unsigned long long seq = 0;

        if (m_pImpl->m_serfThread.sendData(hdr,&channel,seq)) {
            channel.consume();

	        if (channel.m_dataPending) {
                stats = channel.m_data;
                return (channel.m_hdr.Error == "") ? SerfClient::SUCCESS: SerfClient::FAILURE;
            } else {
                m_pImpl->m_serfThread.removeChannel(seq);
                return SerfClient::TIMEOUT;
            }
        }
        return SerfClient::FAILURE;
    }
    
        
    
} // namespace SerfCpp
