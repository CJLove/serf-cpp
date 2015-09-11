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
