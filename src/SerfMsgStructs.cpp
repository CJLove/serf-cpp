#include "SerfMsgStructs.h"

namespace SerfCpp {

    std::ostream &operator<<(std::ostream &os, const ResponseHeader &m)
    {
        os << "Seq: " << m.Seq << " Error: " << m.Error << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Member &m)
    {
        os << std::setw(10) << m.Name << " ";
        // Handle ipv4 vs ipv6 address in the Addr vector
        if (m.Addr[0] == 0x0) {
            os << (unsigned int)(m.Addr[12] & 0xff) << "."
               << (unsigned int)(m.Addr[13] & 0xff) << "."
               << (unsigned int)(m.Addr[14] & 0xff) << "."
               << (unsigned int)(m.Addr[15] & 0xff) << ":" << m.Port;
        } else {
            std::vector<char>::const_iterator i = m.Addr.begin();
            bool colon = false;
            for (; i != m.Addr.end(); i++) {
                os << std::setw(2) << std::hex << std::setfill('0') << static_cast<int>((*i) & 0xff);
                if (colon) os << ":";
                colon = !colon;
            }
            os << std::dec << std::setfill(' ') << m.Port;
        }
        os << " " << m.Status;
        std::map<std::string,std::string>::const_iterator ii = m.Tags.begin();
        for (; ii != m.Tags.end(); ++ii) {
            os << " " << (*ii).first << ":" << (*ii).second;
        }
        os << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const MembersResponse &m)
    {
        std::vector<Member>::const_iterator i = m.Members.begin();
        for (; i != m.Members.end(); ++i) {
            os << *i;
        }
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const MembersFilteredRequest &r)
    {
        std::map<std::string,std::string>::const_iterator i = r.Tags.begin();
        os << "Tags: ";
        for (; i != r.Tags.end(); ++i) {
            os << (*i).first << ":" << (*i).second << " ";
        }
        os << "Status:" << r.Status << " Name:" << r.Name << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const TagsRequest &r)
    {
        std::map<std::string,std::string>::const_iterator i = r.Tags.begin();
        os << "Tags: ";
        for (; i != r.Tags.end(); ++i) {
            os << (*i).first << ":" << (*i).second << " ";
        }
        std::vector<std::string>::const_iterator ii = r.DeleteTags.begin();
        os << "Delete Tags: ";
        for (; ii != r.DeleteTags.end(); ++ii) {
            os << (*ii) << " ";
        }
        os << std::endl;
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

    std::ostream &operator<<(std::ostream &os, const StopRequest &r)
    {
        os << "StopRequest " << r.Stop << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const LogRecord &r)
    {
        os << "LogRecord: " << r.Log << std::endl;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const UserEventRecord &r)
    {
        os << "Event: " << r.Event << " Time: " << r.LTime
           << "\nName: " << r.Name << std::endl;
        // TODO: payload
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const MemberEventRecord &r)
    {
        os << "Event: " << r.Event << std::endl;
        std::vector<Member>::const_iterator i = r.Members.begin();
        for (; i != r.Members.end(); ++i) {
            os << *i;
        }
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const QueryRecord &r)
    {
        os << "Event: " << r.Event << " From: " << r.From;
        // TODO: payload
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const StatsResponse &r)
    {
        os << "Agent: " << std::endl
           << "  name: " << r.agent.name << std::endl
           << "Runtime: " << std::endl
           << "  os: " << r.runtime.os << std::endl
           << "  arch: " << r.runtime.arch << std::endl
           << "  version: " << r.runtime.version << std::endl
           << "  max_procs: " << r.runtime.max_procs << std::endl
           << "  goroutines: " << r.runtime.goroutines << std::endl
           << "Serf:" << std::endl
           << "  failed: " << r.serf.failed << std::endl
           << "  left: " << r.serf.left << std::endl
           << "  event_time: " << r.serf.event_time << std::endl
           << "  query_time: " << r.serf.query_time << std::endl
           << "  event_queue: " << r.serf.event_queue << std::endl
           << "  members: " << r.serf.members << std::endl
           << "  member_time: " << r.serf.member_time << std::endl
           << "  intent_queue: " << r.serf.intent_queue << std::endl
           << "  query_queue: " << r.serf.query_queue << std::endl;
        return os;
    }

    //
    // Debug routine to dump the sequence of encoded msgpack data
    //
    void printMsgPack(std::string const& buf) {
        std::string::const_iterator end = buf.end();
        for (std::string::const_iterator it = buf.begin();
             it != end;
             ++it) {
            std::cout
                << std::setw(2)
                << std::hex
                << std::setfill('0')
                << (static_cast<int>(*it) & 0xff)
                << ' ';
        }
        std::cout << std::dec << std::endl;
    }

} // namespace SerfCpp
