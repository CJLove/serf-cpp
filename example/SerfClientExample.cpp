#include <string>
#include <sstream>
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>


#include "SerfMsgStructs.h"

using namespace SerfCpp;

int main(int argc, char**argv)
{
    std::string ipAddr("127.0.0.1");
    short port = 7373;
    int c;

    while ((c = getopt(argc,argv,"i:p:")) != EOF) {
        switch(c) {
        case 'i':
            ipAddr=optarg;
            break;
        case 'p':
            port = strtoul(optarg,NULL,10);
            break;
        }
    }

    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;

    if (sock == -1) {
        perror("Couldn't create socket");
        exit(1);
    }
        

    server.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    server.sin_family = AF_INET;
    server.sin_port=htons(port);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("connect failed");
        exit(1);
    }

    {
        RequestHeader req;
        HandshakeRequest hs;

        req.Command="handshake";
        req.Seq=0;
        hs.Version=1;

        std::stringstream ss;
        msgpack::pack(ss,req);
        msgpack::pack(ss,hs);
        printMsgPack(ss.str());


        if (send(sock,ss.str().data(),ss.str().size(),0) < 0) {
            perror("send failed");
            exit(1);
        }
    }

    {
        char buffer[2048];
        int size;
        size = recv(sock,buffer,sizeof(buffer), 0);
        if (size < 0) {
            perror("recv failed");
        }
        msgpack::unpacked unp;
        msgpack::unpack(unp, buffer,size);
        msgpack::object obj = unp.get();
        std::cout << obj << std::endl;
    }

    {
        RequestHeader req;
        JoinRequest join;

        req.Command="join";
        req.Seq=1;
        join.Existing.push_back("192.168.0.211");
        join.Replay = false;

        std::stringstream ss;
        msgpack::pack(ss,req);
        msgpack::pack(ss,join);
        printMsgPack(ss.str());


        if (send(sock,ss.str().data(),ss.str().size(),0) < 0) {
            perror("send failed");
            exit(1);
        }
    }
    
    {
        char buffer[2048];
        int size;
        size = recv(sock,buffer,sizeof(buffer), 0);
        if (size < 0) {
            perror("recv failed");
        }
        msgpack::unpacker pac;
        pac.reserve_buffer(size);
        memcpy(pac.buffer(),buffer,size);
        pac.buffer_consumed(size);

        msgpack::unpacked unp;
        while(pac.next(&unp)) {
            msgpack::object obj = unp.get();
            std::cout << obj << std::endl;
        }
        
    }

    {
        RequestHeader req;

        req.Command="members";
        req.Seq=2;

        std::stringstream ss;
        msgpack::pack(ss,req);
        printMsgPack(ss.str());

        if (send(sock,ss.str().data(),ss.str().size(),0) < 0) {
            perror("send failed");
            exit(1);
        }
    }
    
    {
        char buffer[2048];
        int size;
        size = recv(sock,buffer,sizeof(buffer), 0);
        if (size < 0) {
            perror("recv failed");
        }
        msgpack::unpacker pac;
        pac.reserve_buffer(size);
        memcpy(pac.buffer(),buffer,size);
        pac.buffer_consumed(size);

        ResponseHeader h;
        MembersResponse resp;
        msgpack::unpacked unp;
        if (pac.next(&unp)) {
            msgpack::object obj = unp.get();
            h = obj.as<ResponseHeader>();
        }
        if (pac.next(&unp)) {
            msgpack::object obj = unp.get();
            std::cout << obj << std::endl;
            resp = obj.as<MembersResponse>();
            Member m = resp.Members[0];

            int a = 5;
            
        }
    }
    
}
    
