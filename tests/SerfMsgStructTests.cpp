#include <iostream>

#include "SerfMsgStructs.h"
#include "gtest/gtest.h"

TEST(SerfMsgStructTest, requestHeader)
{
    RequestHeader hdr;
    hdr.Command="handshake";
    hdr.Seq=0;

    std::stringstream ss;
    msgpack::pack(ss, hdr);

    printMsgPack(ss.str());

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    std::cout << obj << std::endl;
    EXPECT_EQ(obj.as<RequestHeader>(),hdr);
}
