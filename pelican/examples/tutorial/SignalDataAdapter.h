#ifndef SIGNALDATAADAPTER_H
#define SIGNALDATAADAPTER_H

#include "pelican/core/AbstractStreamAdapter.h"

using namespace pelican;

/*
 * Adapter to convert chunks of signal stream data into a SignalData data-blob.
 */
class SignalDataAdapter : public AbstractStreamAdapter
{
    public:
        // Constructs the adapter.
        SignalDataAdapter(const ConfigNode& config);

        // Method to deserialise chunks of memory provided by the I/O device.
        void deserialise(QIODevice* device);

    private:
        static const unsigned _headerSize = 32;
        unsigned _samplesPerPacket;
        unsigned _packetSize;
};

PELICAN_DECLARE_ADAPTER(SignalDataAdapter)

#endif // SIGNALDATAADAPTER_H
