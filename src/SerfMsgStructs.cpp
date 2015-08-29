#include "SerfMsgStructs.h"

namespace SerfCpp {

//
// Debug routine to dump the sequence of encoded msgpack data
//
void printMsgPack(std::string const& buf) {
    std::string::const_iterator end = buf.end();
    for (std::string::const_iterator it = buf.begin();
         it != end;
         ++it) {
        std::cout
            << std::setw(2)
            << std::hex
            << std::setfill('0')
            << (static_cast<int>(*it) & 0xff)
            << ' ';
    }
    std::cout << std::dec << std::endl;
}


} // namespace SerfCpp
