
#include "SerfChannel.h"
#include "SerfIoThread.h"
#include "serf-cpp/ISerfListener.h"

namespace SerfCpp {

    ChannelBase::ChannelBase(ChannelType type): m_type(type)
    {}

    LogChannel::LogChannel(ISerfLogListener* listener):
        ChannelBase(ChannelBase::LOG), m_listener(listener)
    {
    }

    void LogChannel::produce(ResponseHeader &hdr, msgpack::unpacker &unpacker)
    {
        LogRecord record;

        // Unpack the payload
        msgpack::object_handle oh;
        if (unpacker.next(oh)) {
            msgpack::object obj = oh.get();
            record = obj.as<LogRecord>();
        }

        m_listener->onLogRecord(hdr,record);
    }

    QueryChannel::QueryChannel(SerfIoThread &ioThread,
                               ISerfQueryListener* listener):
        ChannelBase(ChannelBase::QUERY), m_ioThread(ioThread), m_listener(listener)
    {
    }

    void QueryChannel::produce(ResponseHeader &hdr, msgpack::unpacker &unpacker)
    {
        // Unpack the payload
        msgpack::object_handle oh;
        if (unpacker.next(oh)) {
            msgpack::object obj = oh.get();

            NodeResponseBase type;

            try {
                obj.convert(type);
                if (type.Type == "ack") {
                    NodeAck ack;
                    obj.convert(ack);
                    m_listener->onQueryAck(hdr,ack);
                } else if (type.Type == "response") {
                    NodeResponse resp;
                    obj.convert(resp);
                    m_listener->onQueryResponse(hdr,resp);
                } else if (type.Type == "done") {
                    m_listener->onQueryComplete(hdr);

                    // deregister this channel
                    m_ioThread.removeChannel(hdr.Seq);
                }
            } catch (std::exception &e) {
                std::cout << "Unexpected node response type " << type.Type << " "
                          << e.what() << std::endl;
            }
        }

    }

    EventChannel::EventChannel(ISerfEventListener* listener):
        ChannelBase(ChannelBase::EVENT), m_listener(listener)
    {
    }

    void EventChannel::produce(ResponseHeader &hdr, msgpack::unpacker &unpacker)
    {

        // Unpack the payload
        msgpack::object_handle oh;
        if (unpacker.next(oh)) {
            msgpack::object obj = oh.get();
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
