#include <string>
#include <sstream>
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "SerfClient.h"
#include "SerfMsgStructs.h"
#include "ISerfListener.h"

using namespace SerfCpp;

class EventListener: public ISerfEventListener {
public:
    EventListener()
    {}

    ~EventListener()
    {}

    void onUserEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::UserEventRecord &record);

    void onMemberEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::MemberEventRecord &record);

    void onQueryEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::QueryRecord &record);
};

void
EventListener::onUserEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::UserEventRecord &record)
{
    std::cout << "UserEvent Hdr: " << hdr << std::endl << record;
}

void
EventListener::onMemberEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::MemberEventRecord &record)
{
    std::cout << "MemberEvent Hdr: " << hdr << std::endl << record;
}

void
EventListener::onQueryEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::QueryRecord &record)
{
    std::cout << "QueryEvent Hdr: " << hdr << std::endl << record;
}
        

class LogListener: public ISerfLogListener {
public:
    LogListener()
    {}

    ~LogListener()
    {}

    void onLogRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::LogRecord &record);
};

void
LogListener::onLogRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::LogRecord &record)
{
    std::cout << "Seq: " << hdr.Seq << " " << record.Log << std::endl;
}

int main(int argc, char**argv)
{
    if (argc < 2) {
        std::cout << "Usage:" << std::endl
                  << "SerfClientExample <command> [args] ..." << std::endl;
        exit(1);
    }
    
    SerfClient client;
    //    bool result = client.Connect(ipAddr,port);
    SerfClient::SerfResponse resp = client.Connect();

    if (resp != SerfClient::SUCCESS) {
        std::cout << "Failed to connect to serf agent" << std::endl;
        exit(1);
    }

    std::string command = argv[1];

    if (command == "join") {
        std::vector<std::string> addrs;
        int count = 0;
        
        for (int i = 2; i < argc; ++i) {
            addrs.push_back(argv[i]);
        }

        resp = client.Join(addrs,false,count);

        std::cout << "Join response:" << resp << std::endl
                  << "Join node count: " << count << std::endl;

    } else if (command == "members") {

        MembersResponse members;

        resp = client.Members(members);

        std::cout << "Members response:" << resp << std::endl
                  << "Members:\n" << members << std::endl;

    } else if (command == "event") {
        std::string name = argv[2];

        std::vector<char> payload;
        if (argc > 4) {
            char *c = argv[3];
            while (*c != '\0') {
                payload.push_back(*c);
                c++;
            }
        }

        resp = client.Event(name,payload,false);

        std::cout << "Event response:" << resp << std::endl
                  << "Event:" << name << std::endl;
    } else if (command == "force-leave") {
        std::string name = argv[2];

        resp = client.ForceLeave(name);

        std::cout << "ForceLeave response:" << resp << std::endl
                  << "ForceLeave:" << name << std::endl;
    } else if (command == "leave") {

        resp = client.Leave();

        std::cout << "Leave result:" << resp << std::endl;
    } else if (command == "monitor") {
        LogListener listener;
        unsigned long long seq = 0ULL;

        resp = client.Monitor("Debug",&listener,seq);

        if (seq != 0) {
            std::cout << "Listening to log records for 30 seconds..." << std::endl;
            sleep(30);

            std::cout << "Stopping Monitor registration for Seq=" << seq << std::endl;

            resp = client.Stop(seq);

            std::cout << "Stop result: " << resp << std::endl;

        }

    } else if (command == "stream") {
        EventListener listener;
        unsigned long long seq = 0ULL;

        resp = client.Stream("*",&listener,seq);

        if (seq != 0) {
            std::cout << "Listening to events for 60 seconds..." << std::endl;
            sleep(60);

            std::cout << "Stopping Stream registration for Seq=" << seq << std::endl;

            resp = client.Stop(seq);

            std::cout << "Stop result: " << resp << std::endl;
        }
    } else if (command == "stats") {
        StatsResponse stats;
        resp = client.Stats(stats);

        std::cout << "Stats result:" << resp << std::endl << stats;
    }           

    // Close the connection
    std::cout << "Closing socket connection to serf agent" << std::endl;
    resp = client.Close();
    std::cout << "Close result:" << resp << std::endl;

    exit(0);

}
    
