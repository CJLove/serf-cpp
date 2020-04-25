#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <cstdlib>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include "serf-cpp/ISerfListener.h"
#include "serf-cpp/SerfClient.h"
#include "serf-cpp/SerfMsgStructs.h"

using namespace SerfCpp; // NOLINT - ignore since this is sample code

static SerfStringArray split(const std::string &str, char delimiter) {
    SerfStringArray internal;
    std::stringstream ss(str);  // Turn the string into a stream.
    std::string tok;

    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}

class EventListener : public ISerfEventListener {
public:
    EventListener() = delete;
    explicit EventListener(SerfClient &client) : m_client(client) {}

    ~EventListener() override = default;

    EventListener(const EventListener &) = delete;
    EventListener(const EventListener &&) = delete;
    void operator=(const EventListener &) = delete;
    void operator=(const EventListener &&) = delete;

    void onUserEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::UserEventRecord &record) override;

    void onMemberEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::MemberEventRecord &record) override;

    void onQueryEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::QueryRecord &record) override;

private:
    SerfClient & m_client;
};

void EventListener::onUserEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::UserEventRecord &record) {
    std::cout << "\n==> Received UserEvent Hdr: " << hdr << record;
}

void EventListener::onMemberEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::MemberEventRecord &record) {
    std::cout << "\n==> Received MemberEvent Hdr: " << hdr << record;
}

void EventListener::onQueryEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::QueryRecord &record) {
    std::cout << "\n==> Received QueryEvent Hdr: " << hdr << record;

    // Auto respond to "testQuery"
    if (record.Name == "testQuery") {
        SerfPayload payload;
        payload.push_back('T');
        payload.push_back('E');
        payload.push_back('S');
        payload.push_back('T');

        SerfClient::SerfResponse resp = m_client.Respond(record.ID, payload);
        std::cout << "\n==> Auto response: " << resp << "\n";
    }
}

class QueryListener : public ISerfQueryListener {
public:
    QueryListener() = default;
    QueryListener(const QueryListener &) = delete;
    QueryListener(const QueryListener &&) = delete;

    ~QueryListener() override = default;

    void operator=(const QueryListener &) = delete;
    void operator=(const QueryListener &&) = delete;

    void onQueryAck(SerfCpp::ResponseHeader &hdr, SerfCpp::NodeAck &ack) override;

    void onQueryResponse(SerfCpp::ResponseHeader &hdr, SerfCpp::NodeResponse &resp) override;

    void onQueryComplete(SerfCpp::ResponseHeader &hdr) override;

private:
    int m_acks = 0;
    int m_responses = 0;
};

void QueryListener::onQueryAck(SerfCpp::ResponseHeader &hdr, SerfCpp::NodeAck &ack) {
    std::cout << "\n==> NodeResponse Hdr: " << hdr << ack;
    m_acks++;
}

void QueryListener::onQueryResponse(SerfCpp::ResponseHeader &hdr, SerfCpp::NodeResponse &resp) {
    std::cout << "\n==> NodeResponse Hdr: " << hdr << resp;
    m_responses++;
}

void QueryListener::onQueryComplete(SerfCpp::ResponseHeader & /* hdr */ ) {
    std::cout << "\n==> Query Complete:  Acks: " << m_acks << " Responses: " << m_responses << "\n";
    m_acks = 0;
    m_responses = 0;
}

class LogListener : public ISerfLogListener {
public:
    LogListener() = default;
    LogListener(const LogListener &) = delete;
    LogListener(const LogListener &&) = delete;

    ~LogListener() override = default;

    void operator=(const LogListener &) = delete;
    void operator=(const LogListener &&) = delete;

    void onLogRecord(SerfCpp::ResponseHeader &hdr, const SerfCpp::LogRecord &record) override;
};

void LogListener::onLogRecord(SerfCpp::ResponseHeader &hdr, const SerfCpp::LogRecord &record) {
    std::cout << "\n==> Log Record Seq: " << hdr.Seq << " " << record.Log << "\n";
}

int main(int argc, char **argv) {
    int c = 0;
    std::string host = "127.0.0.1";
    uint16_t port = 7373;

    while ((c = getopt(argc, argv, "h:p:?")) != EOF) {
        switch (c) {
            case 'h':
                host = optarg;
                break;
            case 'p':
                port = static_cast<uint16_t>(std::strtoul(optarg,nullptr,10));
                break;
            case '?':
            default:
                std::cerr << argv[0] << " [-h <host>][-p <port>]\n"; // NOLINT - clang-tidy ignore
                exit(1);
        }
    }

    SerfClient client;
    LogListener logListener;
    EventListener eventListener(client);
    QueryListener queryListener;
    SerfClient::SerfResponse resp = client.Connect(host, port);

    if (resp != SerfClient::SUCCESS) {
        std::cout << "Failed to connect to serf agent" << "\n";
        exit(1);
    }

    while (true) {

        std::string line;
        std::cout << "Command > ";
        getline(std::cin, line);

        SerfStringArray args = ::split(line, ' ');
        if (args.empty()) {
            continue;
        }

        std::string command = args[0];
        args.erase(args.begin());

        if (command == "quit") {
            break;
        } else if (command == "help") {
            std::cout << "Commands:\n"
                      << "    join <addr> <addr> <addr> ...\n"
                      << "    auth <key>\n"
                      << "    install <key>\n"
                      << "    use <key>\n"
                      << "    remove <key>\n"
                      << "    list\n"
                      << "    members\n"
                      << "    event <name> <payload>\n"
                      << "    force-leave\n"
                      << "    leave\n"
                      << "    coord <name>\n"
                      << "    stop <seq>\n"
                      << "    respond <id> <payload>\n"
                      << "    monitor\n"
                      << "    stream\n"
                      << "    tag <tag>:value\n"
                      << "    rmtag <tag>\n"
                      << "    query <name> <payload>\n"
                      << "    stats\n"
                      << "    version\n";
        } else if (command == "version") {
            uint32_t major = 0, minor = 0, patch = 0;
            client.Version(major,minor,patch);
            std::cout << "serf-cpp version " << major << "." << minor << "." << patch << "\n";
        } else if (command == "join") {
            int count = 0;
            resp = client.Join(args, false, count);

            std::cout << "Join response:" << resp << "\nJoin node count: " << count << "\n";

        } else if (command == "auth") {
            std::string key = args[1];

            resp = client.Auth(key);
            std::cout << "Auth response:" << resp << "\n";
        } else if (command == "use") {
            std::string key = args[1];
            resp = client.UseKey(key);

            std::cout << "Use Key response:" << resp << "\n";
        } else if (command == "install") {
            std::string key = args[1];
            KeyResponse keys;
            resp = client.InstallKey(key, keys);

            std::cout << "Install response:" << resp << "\n" << keys << "\n";
        } else if (command == "remove") {
            std::string key = args[1];
            KeyResponse keys;
            resp = client.RemoveKey(key, keys);

            std::cout << "Remove response:" << resp << "\n" << keys << "\n";
        } else if (command == "list") {
            KeyListResponse keys;

            resp = client.ListKeys(keys);

            std::cout << "List Keys response:" << resp << "\n" << keys << "\n";
        } else if (command == "members") {

            MembersResponse members;

            resp = client.Members(members);

            std::cout << "Members response:" << resp << "\n" << members << "\n";
        } else if (command == "tag") {
            std::string tagString = args[0];
            SerfStringMap tags;
            SerfStringArray rmtags;
            size_t sep = tagString.find(':');
            if (sep != std::string::npos) {
                std::string tag = tagString.substr(0,sep);
                std::string val = tagString.substr(sep+1);
                tags[tag] = val;

                resp = client.Tags(tags,rmtags);
                std::cout << "Tags response:" << resp << "\n";
            } else {
                std::cerr << "Invalid tag specification\n";
            }
        } else if (command == "rmtag") {
            SerfStringMap tags;
            SerfStringArray rmtags;
            rmtags.push_back(args[0]);
            resp = client.Tags(tags,rmtags);
            std::cout << "RmTags response: " << resp << "\n";
        } else if (command == "query") {
            std::string name = args[0];
            SerfPayload payload;

            if (args.size() > 1) {
                for (const auto &ch: args[1]) {
                    payload.push_back(ch);
                }
            }

            // Expect acks, 120 second timeout
            resp = client.Query(name, payload, &queryListener, true, 120000000000ULL);
            std::cout << "Query response:" << resp << "\n";
        } else if (command == "coord") {
            std::string node = args[0];
            CoordResponse coord;

            resp = client.GetCoordinate(node, coord);
            std::cout << "GetCoordinate response:" << resp << "\n" << coord << "\n";
        } else if (command == "event") {
            std::string name = args[0];

            SerfPayload payload;

            if (args.size() > 1) {
                for (const auto &ch: args[1]) {
                    payload.push_back(ch);
                }
            }

            resp = client.Event(name, payload, true);

            std::cout << "Event response:" << resp << "\n" << "Event:" << name << "\n";
        } else if (command == "respond") {
            uint64_t id = strtoull(args[0].c_str(), nullptr, 10);
            SerfPayload payload;

            if (args.size() > 1) {
                for (const auto &ch: args[1]) {
                    payload.push_back(ch);
                }
            }
            resp = client.Respond(id, payload);
            std::cout << "Respond response:" << resp << " for ID: " << id << "\n";

        } else if (command == "force-leave") {
            std::string name = args[0];

            resp = client.ForceLeave(name);

            std::cout << "ForceLeave response:" << resp << "\n" << "ForceLeave:" << name << "\n";
        } else if (command == "leave") {

            resp = client.Leave();

            std::cout << "Leave result:" << resp << "\n";
        } else if (command == "stop") {
            uint64_t seq = strtoull(args[0].c_str(), nullptr, 10);

            std::cout << "Stopping Monitor/Stream registration for Seq=" << seq << "\n";

            resp = client.Stop(seq);

            std::cout << "Stop result: " << resp << "\n";

        } else if (command == "monitor") {

            uint64_t seq = 0ULL;

            resp = client.Monitor("Debug", &logListener, seq);

            std::cout << "monitor result: " << resp << "\n";
            if (seq != 0) {
                std::cout << "Listening to log records for Seq=" << seq << "\n";
            }

        } else if (command == "stream") {
            uint64_t seq = 0ULL;

            resp = client.Stream("*", &eventListener, seq);

            std::cout << "stream result: " << resp << "\n";
            if (seq != 0) {
                std::cout << "Listening to events for Seq=" << seq << "\n";
            }
        } else if (command == "stats") {
            StatsResponse stats;
            resp = client.Stats(stats);

            std::cout << "Stats result:" << resp << "\n" << stats;
        }
    }
    // Close the connection
    std::cout << "Closing socket connection to serf agent" << "\n";
    resp = client.Close();
    std::cout << "Close result:" << resp << "\n";

    exit(0);
}
