#include <string>
#include <sstream>
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "SerfClient.h"
#include "SerfMsgStructs.h"

using namespace SerfCpp;

int main(int argc, char**argv)
{
    if (argc < 2) {
        std::cout << "Usage:" << std::endl
                  << "SerfClientExample <command> [args] ..." << std::endl;
        exit(1);
    }
    
    SerfClient client;
    //    bool result = client.Connect(ipAddr,port);
    bool result = client.Connect();

    if (result == false) {
        std::cout << "Failed to connect to serf agent" << std::endl;
        exit(1);
    }

    std::string command = argv[1];

    if (command == "join") {
        std::vector<std::string> addrs;
        
        for (int i = 2; i < argc; ++i) {
            addrs.push_back(argv[i]);
        }

        int count = client.Join(addrs,false);

        std::cout << "Join node count:\n" << count << std::endl;

    } else if (command == "members") {

        MembersResponse resp = client.Members();

        std::cout << "Members:\n" << resp << std::endl;

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
        result = client.Event(name,payload,false);

        std::cout << "Event " << name << " result: " << (result ? "true" : "false") << std::endl;
    } else if (command == "force-leave") {
        std::string name = argv[2];

        result = client.ForceLeave(name);

        std::cout << "ForceLeave " << name << " result: " << (result ? "true" : "false") << std::endl;
    } else if (command == "leave") {

        result = client.Leave();

        std::cout << "Leave result: " << (result ? "true" : "false") << std::endl;
    }
           

    // Close the connection
    std::cout << "Closing socket connection to serf agent" << std::endl;
    result = client.Close();
    std::cout << "Close result: " << (result ? "true" : "false") << std::endl;

    exit(0);

}
    
