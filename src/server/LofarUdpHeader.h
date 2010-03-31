#ifndef LOFARUDPHEADER_H_
#define LOFARUDPHEADER_H_

#include <complex>
#include <math.h>
// #include <cstdint> causes a c++0x error (although needed for uint8_t etc) !

namespace pelican {

// All data is in Little Endian format!
struct UDPPacket {
        struct Header {
                uint8_t  version;
                uint8_t  sourceInfo;
                uint16_t configuration;
                uint16_t station;
                uint8_t  nrBeamlets;
                uint8_t  nrBlocks;
                uint32_t timestamp;
                uint32_t blockSequenceNumber;
        } header;

        char       data[8130];
};

} // namespace pelican

#endif /* LOFARUDPHEADER_H_ */
