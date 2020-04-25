#include "serf-cpp/SerfClient.h"
#include "SerfChannel.h"
#include "SerfIoThread.h"

namespace SerfCpp {
//
// Private Implementation
//
struct SerfClient::SerfClientImpl {

    SerfIoThread m_serfThread;

    const SerfClient &m_parent;

    explicit SerfClientImpl(const SerfClient &parent) : m_parent(parent) {}
};

//
// Public Implementation
//
std::ostream &operator<<(std::ostream &os, const SerfClient::SerfResponse &r) {
    switch (r) {
        case SerfClient::SUCCESS:
            os << "SUCCESS";
            break;
        case SerfClient::FAILURE:
            os << "FAILURE";
            break;
        case SerfClient::TIMEOUT:
            os << "TIMEOUT";
            break;
    }
    return os;
}

SerfClient::SerfClient() : m_pImpl(new SerfClientImpl(*this)) {}

SerfClient::~SerfClient() {
    m_pImpl->m_serfThread.Close();
}

void SerfClient::Version(uint32_t &major, uint32_t &minor, uint32_t &patch) {
    major = SERF_CPP_VERSION_MAJOR;
    minor = SERF_CPP_VERSION_MINOR;
    patch = SERF_CPP_VERSION_PATCH;
}

SerfClient::SerfResponse SerfClient::Connect(const std::string &ipAddr, const uint16_t &port) {
    return (m_pImpl->m_serfThread.Connect(ipAddr, port)) ? SerfClient::SUCCESS : SerfClient::FAILURE;
}

bool SerfClient::IsConnected() { return (m_pImpl->m_serfThread.IsConnected()); }

SerfClient::SerfResponse SerfClient::Close() {
    return (m_pImpl->m_serfThread.Close()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::Join(const SerfStringArray &addrs, bool replay, int &nodeCount) {
    RequestHeader hdr;
    hdr.Command = "join";
    JoinRequest join;
    join.Existing = addrs;
    join.Replay = replay;

    // Channel for receiving response
    ResultChannel<JoinResponse> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, join, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            nodeCount = channel.m_data.Num;
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::Auth(const std::string &authKey) {
    RequestHeader hdr;
    hdr.Command = "auth";
    AuthRequest req;
    req.AuthKey = authKey;

    // Channel for receiving response
    ResultChannel<bool> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::InstallKey(const std::string &key, KeyResponse &keys) {
    RequestHeader hdr;
    hdr.Command = "install-key";
    KeyRequest req;
    req.Key = key;

    // Channel for receiving response
    ResultChannel<KeyResponse> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            keys = channel.m_data;
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::ListKeys(KeyListResponse &keys) {
    RequestHeader hdr;
    hdr.Command = "list-keys";

    // Channel for receiving response
    ResultChannel<KeyListResponse> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            keys = channel.m_data;
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::UseKey(const std::string &key) {
    RequestHeader hdr;
    hdr.Command = "use-key";
    KeyRequest req;
    req.Key = key;

    // Channel for receiving response
    ResultChannel<bool> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::RemoveKey(const std::string &key, KeyResponse &keys) {
    RequestHeader hdr;
    hdr.Command = "remove-key";
    KeyRequest req;
    req.Key = key;

    // Channel for receiving response
    ResultChannel<KeyResponse> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            keys = channel.m_data;
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::Members(MembersResponse &members) {
    RequestHeader hdr;
    hdr.Command = "members";

    // Channel for receiving response
    ResultChannel<MembersResponse> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            members = channel.m_data;
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::MembersFiltered(const SerfStringMap &tags, const std::string &status,
                                                     const std::string &name, MembersResponse &members) {
    RequestHeader hdr;
    hdr.Command = "members-filtered";
    MembersFilteredRequest req;
    req.Tags = tags;
    req.Status = status;
    req.Name = name;
    ;

    // Channel for receiving response
    ResultChannel<MembersResponse> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            members = channel.m_data;
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::Event(const std::string &name, const SerfPayload &payload, bool coalesce) {
    RequestHeader hdr;
    EventRequest event;
    hdr.Command = "event";
    event.Name = name;
    event.Payload = payload;
    event.Coalesce = coalesce;

    // Channel for receiving response
    ResultChannel<bool> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, event, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::Respond(const uint64_t &id, const SerfPayload &payload) {
    RequestHeader hdr;
    hdr.Command = "respond";
    RespondRequest req;
    req.ID = id;
    req.Payload = payload;
    // Channel for receiving response
    ResultChannel<bool> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::ForceLeave(const std::string &nodeName) {
    RequestHeader hdr;
    ForceLeaveRequest req;
    hdr.Command = "force-leave";
    req.Node = nodeName;

    // Channel for receiving response
    ResultChannel<bool> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::Tags(const SerfStringMap &tags, const SerfStringArray &deleteTags) {
    RequestHeader hdr;
    TagsRequest req;
    hdr.Command = "tags";
    req.Tags = tags;
    req.DeleteTags = deleteTags;

    // Channel for receiving response
    ResultChannel<bool> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::Leave() {
    RequestHeader hdr;
    hdr.Command = "leave";

    // Channel for receiving response
    ResultChannel<bool> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::Monitor(const std::string &level, ISerfLogListener *listener, uint64_t &seq) {
    RequestHeader hdr;
    hdr.Command = "monitor";
    MonitorRequest req;
    req.LogLevel = level;

    // Channel for receiving response
    ResultChannel<bool> channel;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            SerfClient::SerfResponse resp = (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;

            if (resp == SerfClient::SUCCESS) {
                m_pImpl->m_serfThread.addLogChannel(channel.m_hdr.Seq, listener);
                seq = channel.m_hdr.Seq;
            }
            return resp;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::Query(const std::string &name, const SerfPayload &payload, ISerfQueryListener *listener,
                                           // Following args are optional
                                           bool requestAck, uint64_t timeout, SerfStringArray *filterNodes,
                                           SerfStringMap *filterTags) {
    RequestHeader hdr;
    hdr.Command = "query";
    QueryRequest req;
    req.Name = name;
    req.Payload = payload;
    req.RequestAck = requestAck;
    req.Timeout = timeout;
    if (filterNodes != nullptr) {
        req.FilterNodes = *filterNodes;
    }
    if (filterTags != nullptr) {
        req.FilterTags = *filterTags;
    }

    // Channel for receiving response
    ResultChannel<bool> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            SerfClient::SerfResponse resp = (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;

            if (resp == SerfClient::SUCCESS) {
                m_pImpl->m_serfThread.addQueryChannel(channel.m_hdr.Seq, listener);
            }
            return resp;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::Stream(const std::string &type, ISerfEventListener *listener, uint64_t &seq) {
    RequestHeader hdr;
    hdr.Command = "stream";
    StreamRequest req;
    req.Type = type;

    // Channel for receiving response
    ResultChannel<bool> channel;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            SerfClient::SerfResponse resp = (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;

            if (resp == SerfClient::SUCCESS) {
                m_pImpl->m_serfThread.addEventChannel(channel.m_hdr.Seq, listener);
                seq = channel.m_hdr.Seq;
            }
            return resp;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::Stop(const uint64_t &stopSeq) {
    RequestHeader hdr;
    StopRequest req;
    hdr.Command = "stop";
    req.Stop = stopSeq;

    // Channel for receiving response
    ResultChannel<bool> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {

        channel.consume();

        if (channel.m_dataPending) {
            SerfClient::SerfResponse resp = (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;

            if (resp == SerfClient::SUCCESS) {
                m_pImpl->m_serfThread.removeChannel(stopSeq);
            }
            return resp;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::Stats(StatsResponse &stats) {
    RequestHeader hdr;
    TagsRequest req;
    hdr.Command = "stats";

    // Channel for receiving response
    ResultChannel<StatsResponse> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            stats = channel.m_data;
            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

SerfClient::SerfResponse SerfClient::GetCoordinate(const std::string &node, CoordResponse &coordinate) {
    RequestHeader hdr;
    hdr.Command = "get-coordinate";
    CoordRequest req;
    req.Node = node;

    // Channel for receiving response
    ResultChannel<CoordResponse> channel;
    uint64_t seq = 0;

    if (m_pImpl->m_serfThread.sendData(hdr, req, &channel, seq)) {
        channel.consume();

        if (channel.m_dataPending) {
            coordinate = channel.m_data;

            return (channel.m_hdr.Error.empty()) ? SerfClient::SUCCESS : SerfClient::FAILURE;
        } else {
            m_pImpl->m_serfThread.removeChannel(seq);
            return SerfClient::TIMEOUT;
        }
    }
    return SerfClient::FAILURE;
}

}  // namespace SerfCpp
