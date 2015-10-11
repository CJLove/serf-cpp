#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "SerfClient.h"
#include "SerfMsgStructs.h"
#include "ISerfListener.h"

using namespace SerfCpp;


std::vector<std::string>
split(const std::string &str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str); // Turn the string into a stream.
    std::string tok;
  
    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }
  
    return internal;
}


class EventListener: public ISerfEventListener {
public:
    EventListener()
    {}

    ~EventListener()
    {}

    virtual void onUserEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::UserEventRecord &record);

    virtual void onMemberEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::MemberEventRecord &record);

    virtual void onQueryEventRecord(SerfCpp::ResponseHeader &hdr, SerfCpp::QueryRecord &record);
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
    //    if (argc < 2) {
    //        std::cout << "Usage:" << std::endl
    //                  << "SerfClientExample <command> [args] ..." << std::endl;
    //        exit(1);
    //    }
    
    SerfClient client;
    LogListener logListener;
    EventListener eventListener;    
    SerfClient::SerfResponse resp = client.Connect();

    if (resp != SerfClient::SUCCESS) {
        std::cout << "Failed to connect to serf agent" << std::endl;
        exit(1);
    }

    while(true) {

        std::string line;
        std::cout << "Command > ";
        getline(std::cin,line);

        std::vector<std::string> args = ::split(line,' ');
        if (args.empty()) continue;
        
        std::string command = args[0];
        args.erase(args.begin());

        if (command == "quit") {
            break;
        } else if (command == "help") {
            std::cout << "Commands:" << std::endl
                      << "    join <addr> <addr> <addr> ..." << std::endl
                      << "    members" << std::endl
                      << "    event <name> <payload>" << std::endl
                      << "    force-leave" << std::endl
                      << "    leave" << std::endl
                      << "    stop <seq>" << std::endl
                      << "    monitor" << std::endl
                      << "    stream" << std::endl
                      << "    stats" << std::endl;
        } else if (command == "join") {
            int count = 0;        
            resp = client.Join(args,false,count);

            std::cout << "Join response:" << resp << std::endl
                      << "Join node count: " << count << std::endl;

        } else if (command == "members") {

            MembersResponse members;

            resp = client.Members(members);

            std::cout << "Members response:" << resp << std::endl
                      << "Members:\n" << members << std::endl;

        } else if (command == "event") {
            std::string name = args[0];

            std::vector<char> payload;

            if (args.size() > 2) {
                const char *c = args[1].c_str();
                while (*c != '\0') {
                    payload.push_back(*c);
                    c++;
                }
            }

            resp = client.Event(name,payload,false);

            std::cout << "Event response:" << resp << std::endl
                      << "Event:" << name << std::endl;
        } else if (command == "force-leave") {
            std::string name = args[0];

            resp = client.ForceLeave(name);

            std::cout << "ForceLeave response:" << resp << std::endl
                      << "ForceLeave:" << name << std::endl;
        } else if (command == "leave") {

            resp = client.Leave();

            std::cout << "Leave result:" << resp << std::endl;
        } else if (command == "stop") {
            unsigned long long seq = strtoull(args[0].c_str(),NULL,10);

            std::cout << "Stopping Monitor/Stream registration for Seq=" << seq << std::endl;

            resp = client.Stop(seq);

            std::cout << "Stop result: " << resp << std::endl;
            
        } else if (command == "monitor") {

            unsigned long long seq = 0ULL;

            resp = client.Monitor("Debug",&logListener,seq);

            if (seq != 0) {
                std::cout << "Listening to log records for Seq=" << seq << std::endl;
            }

        } else if (command == "stream") {
            unsigned long long seq = 0ULL;

            resp = client.Stream("*",&eventListener,seq);

            if (seq != 0) {
                std::cout << "Listening to events for Seq=" << seq << std::endl;
            }
        } else if (command == "stats") {
            StatsResponse stats;
            resp = client.Stats(stats);

            std::cout << "Stats result:" << resp << std::endl << stats;
        }           

    }
    // Close the connection
    std::cout << "Closing socket connection to serf agent" << std::endl;
    resp = client.Close();
    std::cout << "Close result:" << resp << std::endl;

    exit(0);

}
    
