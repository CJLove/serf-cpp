
#include "SerfChannel.h"
#include "ISerfListener.h"

namespace SerfCpp {

    ChannelBase::ChannelBase(ChannelType type): m_type(type)
    {}

    ChannelBase::~ChannelBase()
    {
    }


    LogChannel::LogChannel(ISerfLogListener* listener):
        ChannelBase(ChannelBase::LOG), m_listener(listener)
    {
    }

    LogChannel::~LogChannel()
    {
    }

    void LogChannel::produce(ResponseHeader &hdr, msgpack::unpacker &unpacker)
    {
        LogRecord record;

        // Unpack the payload
        msgpack::unpacked unp;
        if (unpacker.next(&unp)) {
            msgpack::object obj = unp.get();
            record = obj.as<LogRecord>();
        }

        m_listener->onLogRecord(hdr,record);
    }

    EventChannel::EventChannel(ISerfEventListener* listener):
        ChannelBase(ChannelBase::LOG), m_listener(listener)
    {
    }

    EventChannel::~EventChannel()
    {
    }

    void EventChannel::produce(ResponseHeader &hdr, msgpack::unpacker &unpacker)
    {

        // Unpack the payload
        msgpack::unpacked unp;
        if (unpacker.next(&unp)) {
            msgpack::object obj = unp.get();
            RecordBase type;

            try {
                obj.convert(type);

                if (type.Event == "user") {
                    UserEventRecord rec;
                    obj.convert(rec);
	                m_listener->onUserEventRecord(hdr,rec);
                    return;
                } else if (type.Event == "query") {
                    QueryRecord rec;
                    obj.convert(rec);
                    m_listener->onQueryEventRecord(hdr,rec);
                    return;
                } else if (type.Event == "member-join" ||
                           type.Event == "member-leave" ||
                           type.Event == "member-update" ||
                           type.Event == "member-failed" ||
                           type.Event == "member-reap") {
                    MemberEventRecord rec;
                    obj.convert(rec);
                    m_listener->onMemberEventRecord(hdr,rec);
                    return;
                }
            } catch (std::exception &e) {
                std::cout << "Unexpected event type " << type.Event << " " << e.what() << std::endl;
            }
        }
    }
    

} // namespace SerfCpp
