#include "Listeners.h"

using namespace SerfCpp;


void
LogListener::onLogRecord(ResponseHeader &hdr, LogRecord &record)
{
    m_count++;
    m_log = record.Log;
}


void
EventListener::onUserEventRecord(ResponseHeader &hdr, UserEventRecord &record)
{
    m_userCount++;
}

void
EventListener::onMemberEventRecord(ResponseHeader &hdr, MemberEventRecord &record)
{
	m_memberCount++;
}

void
EventListener::onQueryEventRecord(ResponseHeader &hdr, QueryRecord &record)
{
	m_queryCount++;
}


void
QueryListener::onQueryAck(ResponseHeader &hdr, NodeAck &resp)
{
    m_queryAck++;
}

void
QueryListener::onQueryResponse(ResponseHeader &hdr, NodeResponse &resp)
{
    m_queryResp++;    
}

void
QueryListener::onQueryComplete(ResponseHeader &hdr)
{
    m_queryComplete++;    
}
