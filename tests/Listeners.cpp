#include "Listeners.h"

using namespace SerfCpp;


void
LogListener::onLogRecord(ResponseHeader &, const LogRecord &record)
{
    m_count++;
    m_log = record.Log;
}


void
EventListener::onUserEventRecord(ResponseHeader &, UserEventRecord &)
{
    m_userCount++;
}

void
EventListener::onMemberEventRecord(ResponseHeader &, MemberEventRecord &)
{
	m_memberCount++;
}

void
EventListener::onQueryEventRecord(ResponseHeader &, QueryRecord &)
{
	m_queryCount++;
}


void
QueryListener::onQueryAck(ResponseHeader &, NodeAck &)
{
    m_queryAck++;
}

void
QueryListener::onQueryResponse(ResponseHeader &, NodeResponse &)
{
    m_queryResp++;    
}

void
QueryListener::onQueryComplete(ResponseHeader &)
{
    m_queryComplete++;    
}
