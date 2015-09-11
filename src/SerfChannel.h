#pragma once

#include <boost/thread.hpp>

#include "SerfMsgStructs.h"

namespace SerfCpp {

    class ISerfLogListener;
	class ISerfEventListener;    

    struct ChannelBase
    {
        enum ChannelType { REQUEST, LOG, EVENT };
    
        ChannelBase(ChannelType type);

        virtual ~ChannelBase();

        virtual void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) = 0;

        ChannelType m_type;
    };

    struct LogChannel: public ChannelBase {
        LogChannel(ISerfLogListener *);

        ~LogChannel();

        void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker);

        ISerfLogListener *m_listener;
    };

    struct EventChannel: public ChannelBase {
        EventChannel(ISerfEventListener *);

        ~EventChannel();

        void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker);

        ISerfEventListener *m_listener;
    };

    template<typename T> struct ResultChannel: ChannelBase {
    ResultChannel(): ChannelBase(ChannelBase::REQUEST), m_dataPending(false)
            {}
        void consume() {
            boost::posix_time::milliseconds timeoutDuration( 10000 ); //wait for 10 seconds
            boost::unique_lock<boost::mutex> lock(m_mutex);
            while (m_dataPending == false) {
                if (!m_condition.timed_wait(lock,timeoutDuration)) {
                    // Timeout
                    break;
                }
            }
            // Data is now available
        }

        void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) {
            {
                boost::lock_guard<boost::mutex> lock(m_mutex);
                m_hdr = hdr;

                // Unpack the payload
                msgpack::unpacked unp;
                if (unpacker.next(&unp)) {
                    msgpack::object obj = unp.get();
                    m_data = obj.as<T>();
                }
                m_dataPending = true;             
                m_condition.notify_one();
            }
        }

        ResponseHeader m_hdr;
        T m_data;
        bool m_dataPending;
        boost::mutex m_mutex;
        boost::condition_variable m_condition;
    };

    template<>
        struct ResultChannel<bool>: ChannelBase {
    ResultChannel(): ChannelBase(ChannelBase::REQUEST), m_dataPending(false)
            {}
        void consume() {
            boost::posix_time::milliseconds timeoutDuration( 10000 ); //wait for 10 seconds
            boost::unique_lock<boost::mutex> lock(m_mutex);
            while (m_dataPending == false) {
                if (!m_condition.timed_wait(lock,timeoutDuration)) {
                    // Timeout
                    break;
                }
            }
            // Data is now available
        }

        void produce(ResponseHeader &hdr, msgpack::unpacker &unpacker) {
            {
                // No payload to unpack
                boost::lock_guard<boost::mutex> lock(m_mutex);
                m_hdr = hdr;
                m_dataPending = true;             
                m_condition.notify_one();
            }
        }

        ResponseHeader m_hdr;
        bool m_dataPending;
        boost::mutex m_mutex;
        boost::condition_variable m_condition;
    }; 

}	// namespace SerfCpp
