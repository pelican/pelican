#include "tutorial/SignalDataAdapter.h"
#include "tutorial/SignalData.h"

// Construct the signal data adapter.
SignalDataAdapter::SignalDataAdapter(const ConfigNode& config)
    : AbstractStreamAdapter(config)
{
    // Read the configuration using configuration node utility methods.
    _samplesPerPacket = config.getOption("packet", "samples").toUInt();

    // Set up the packet data.
    _packetSize = _headerSize + _samplesPerPacket * sizeof(float);
}

// Called to de-serialise a chunk of data from the input device.
void SignalDataAdapter::deserialise(QIODevice* device)
{
    // A pointer to the data blob to fill should be obtained by calling the
    // dataBlob() inherited method. This returns a pointer to an
    // abstract DataBlob, which should be cast to the appropriate type.
    SignalData* blob = (SignalData*) dataBlob();

    // Set the size of the data blob to fill.
    // The chunk size is obtained by calling the chunkSize() inherited method.
    unsigned packets = chunkSize() / _packetSize;
    blob->resize(packets * _samplesPerPacket);

    // Create a temporary buffer to read out the packet headers, and
    // get the pointer to the data array in the data blob being filled.
    char headerData[_headerSize];
    char* data = (char*) blob->ptr();

    // Loop over the UDP packets in the chunk.
    unsigned bytesRead = 0;
    for (unsigned p = 0; p < packets; ++p)
    {
        // Ensure there is enough data to read from the device.
        while (device->bytesAvailable() < _packetSize)
            device->waitForReadyRead(-1);

        // Read the packet header from the input device and dump it.
        device->read(headerData, _headerSize);

        // Read the packet data from the input device into the data blob.
        bytesRead += device->read(data + bytesRead, _packetSize - _headerSize);
    }
}
