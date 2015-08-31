#pragma once

#include "SerfMsgStructs.h"

namespace SerfCpp {

    class ISerfLogListener {
    public:
        ISerfLogListener();

        virtual ~ISerfLogListener();

        virtual void onLogRecord(ResponseHeader &hdr, LogRecord &record) = 0;
    };

    //class ISerfEventListener {
    //public:


} // namespace SerfCpp
