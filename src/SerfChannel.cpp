
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


} // namespace SerfCpp
