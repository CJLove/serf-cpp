#include <iomanip>

#include "serf-cpp/SerfMsgStructs.h"


namespace SerfCpp {

    //
    // Generate hex dump of a "payload"
    //
    static void dumpPayload(std::ostream &os, const SerfPayload &p)
    {
        const int MAX_COUNT = 8;
        const int MAX_CHAR = 0xff;
        int count = 0;
        std::ios init(nullptr);
	    init.copyfmt(os);
        os << std::hex << std::setfill('0') << std::setw(2);
        auto i = p.begin();
        for (; i != p.end(); ++i, ++count) {
            if ((count != 0) && (count % MAX_COUNT) == 0) {
                os << "\n             ";
            }
            os << "0x" << (static_cast<int>(*i) & MAX_CHAR) << " ";
        }
        os.copyfmt(init);
        os << std::endl;
    }
        
    //
    // Request message structs
    //
    std::ostream &operator<<(std::ostream &os, const MembersFilteredRequest &r)
    {
        auto i = r.Tags.begin();
        os << "Tags: ";
        for (; i != r.Tags.end(); ++i) {
            os << (*i).first << ":" << (*i).second << " ";
        }
        os << "Status:" << r.Status << " Name:" << r.Name << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const TagsRequest &r)
    {
        auto i = r.Tags.begin();
        os << "Tags: ";
        for (; i != r.Tags.end(); ++i) {
            os << (*i).first << ":" << (*i).second << " ";
        }
        auto ii = r.DeleteTags.begin();
        os << "Delete Tags: ";
        for (; ii != r.DeleteTags.end(); ++ii) {
            os << (*ii) << " ";
        }
        os << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const AuthRequest &r)
    {
        os << "AuthKey: " << r.AuthKey;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const KeyRequest &r)
    {
        os << "Key: " << r.Key;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const StreamRequest &r)
    {
        os << "StreamRequest " << r.Type << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const MonitorRequest &r)
    {
        os << "MonitorRequest " << r.LogLevel << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const RespondRequest &r)
    {
        os << "ID: " << r.ID << std::endl;
        // TODO: payload        
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const StopRequest &r)
    {
        os << "StopRequest " << r.Stop << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const CoordRequest &r)
    {
        os << "Node: " << r.Node << std::endl;
        return os;
    }
    

    //
    // Response message structs
    //
    std::ostream &operator<<(std::ostream &os, const ResponseHeader &r)
    {
        os << "[ Seq: " << r.Seq << " Error: \"" << r.Error << "\"]" << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const KeyResponse &r)
    {
        auto i = r.Messages.begin();
        os << "    Messages:" << std::endl;
        for (; i != r.Messages.end(); ++i) {
            os << "      Node: " << (*i).first << " Msg: " << (*i).second << std::endl;
        }
        os << "    NumErr: " << r.NumErr 
           << " NumNodes: " << r.NumNodes 
           << " NumResp: " << r.NumResp << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const KeyListResponse &r)
    {
        auto i = r.Messages.begin();
        os << "    Messages:" << std::endl;
        for (; i != r.Messages.end(); ++i) {
            os << "      Node: " << (*i).first << " Msg: " << (*i).second << std::endl;
        }
        os << "    Keys:" << std::endl;
        auto ii = r.Keys.begin();
        for (; ii != r.Keys.end(); ++ii) {
            os << "      Key: " << (*ii).first << " : " << (*ii).second << std::endl;
        }
        os << "    NumErr: " << r.NumErr 
           << " NumNodes: " << r.NumNodes
           << " NumResp: " << r.NumResp << std::endl;
        return os;
    }
    
    std::ostream &operator<<(std::ostream &os, const Member &r)
    {
        const size_t IPV4_SIZE = 4;
        const size_t IPV6_SIZE = 16;
        const size_t IPV4_OFFSET = 12;
        const uint8_t MAX_OCTET = 0xff;
        const unsigned WIDTH = 16;
        os << std::setw(WIDTH) << r.Name << " ";
        // Handle ipv4 vs ipv6 address in the Addr vector
        size_t size = r.Addr.size();
        if (size == IPV4_SIZE || (size == IPV6_SIZE && r.Addr[0] == 0x0)) {
            // IPv4 address has been observed to be in bytes 12-15
            // of a 16-byte vector or bytes 0-3 of 4-byte vector
            size_t offset = (size == IPV4_SIZE) ? 0 : IPV4_OFFSET;

            os << static_cast<unsigned>(r.Addr[offset] & MAX_OCTET) << "."
               << static_cast<unsigned>(r.Addr[offset+1] & MAX_OCTET) << "."
               << static_cast<unsigned>(r.Addr[offset+2] & MAX_OCTET) << "."
               << static_cast<unsigned>(r.Addr[offset+3] & MAX_OCTET) << ":" << r.Port;
        } else {
            auto i = r.Addr.begin();
            bool colon = false;
            for (; i != r.Addr.end(); i++) {
                os << std::setw(2) << std::hex << std::setfill('0') << static_cast<int>((*i) & MAX_OCTET);
                if (colon) {
                    os << ":";
                }
                colon = !colon;
            }
            os << std::dec << std::setfill(' ') << r.Port;
        }
        os << " " << r.Status;
        auto ii = r.Tags.begin();
        for (; ii != r.Tags.end(); ++ii) {
            os << " " << (*ii).first << ":" << (*ii).second;
        }
        os << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const MembersResponse &r)
    {
        auto i = r.Members.begin();
        os << "    Members:" << std::endl;
        for (; i != r.Members.end(); ++i) {
            os << *i;
        }
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Coordinate &r)
    {
        os << "    Coordinate:" << std::endl
           << "      Adjustment: " << r.Adjustment << std::endl
           << "      Error: " << r.Error << std::endl
           << "      Height: " << r.Height << std::endl
           << "      Vec: ";
        int count=0;
        auto i = r.Vec.begin();
        for (; i != r.Vec.end(); i++, ++count) {
            if ((count != 0) && ((count % 4)==0)) {
                os << "\n          ";
            }
            os << (*i) << " ";
        }
        os << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const CoordResponse &r)
    {
        os << r.Coord
           << "    Ok: " << r.Ok << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const StatsResponse &r)
    {
        os << "    Agent: " << std::endl
           << "      name: " << r.agent.name << std::endl
           << "    Runtime: " << std::endl
           << "      os: " << r.runtime.os << std::endl
           << "      arch: " << r.runtime.arch << std::endl
           << "      version: " << r.runtime.version << std::endl
           << "      max_procs: " << r.runtime.max_procs << std::endl
           << "      goroutines: " << r.runtime.goroutines << std::endl
           << "    Serf:" << std::endl
           << "      failed: " << r.serf.failed << std::endl
           << "      left: " << r.serf.left << std::endl
           << "      event_time: " << r.serf.event_time << std::endl
           << "      query_time: " << r.serf.query_time << std::endl
           << "      event_queue: " << r.serf.event_queue << std::endl
           << "      members: " << r.serf.members << std::endl
           << "      member_time: " << r.serf.member_time << std::endl
           << "      intent_queue: " << r.serf.intent_queue << std::endl
           << "      query_queue: " << r.serf.query_queue << std::endl;
        return os;
    }

    //
    // Async message structs
    //
    std::ostream &operator<<(std::ostream &os, const LogRecord &r)
    {
        os << "    LogRecord: " << r.Log << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const UserEventRecord &r)
    {
        os << "    UserEvent: Time: " << r.LTime << " Name: " << r.Name << std::endl
           << "    Payload: ";
        dumpPayload(os,r.Payload);

        // TODO: payload
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const MemberEventRecord &r)
    {
        os << "    MemberEvent: " << r.Event << std::endl;
        auto i = r.Members.begin();
        for (; i != r.Members.end(); ++i) {
            os << *i;
        }
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const QueryRecord &r)
    {
        os << "    QueryEvent: " << r.Event 
           << " ID: " << r.ID << " Name: " << r.Name
           << " LTime: " << r.LTime << std::endl
           << "    Payload: ";
        dumpPayload(os,r.Payload);
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const NodeResponseBase &r)
    {
        os << "    Node Response Type: " << r.Type << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const NodeAck &r)
    {
        os << "    Node Response Type: " << r.Type << " From: " << r.From << std::endl;
        return os;
    }
    
    std::ostream &operator<<(std::ostream &os, const NodeResponse &r)
    {
        os << "    Node Response Type: " << r.Type << " From: " << r.From << std::endl
           << "    Payload: ";
        dumpPayload(os,r.Payload);
        return os;
    }


    //
    // Debug routine to dump the sequence of encoded msgpack data
    //
    void printMsgPack(std::string const& buf) {
        const int MAX_OCTET = 0xff;

        std::string::const_iterator end = buf.end();
        for (std::string::const_iterator it = buf.begin();
             it != end;
             ++it) {
            std::cout
                << std::setw(2)
                << std::hex
                << std::setfill('0')
                << (static_cast<int>(*it) & MAX_OCTET)
                << ' ';
        }
        std::cout << std::dec << std::endl;
    }

} // namespace SerfCpp