#include "reference/AdapterExample.h"
#include "reference/DataBlobExample.h"

// Construct the example adapter.
AdapterExample::AdapterExample(const ConfigNode& config)
    : AbstractStreamAdapter(config)
{
    // Read the configuration using configuration node utility methods.
    _nBitsPerSample = config.getOption("samples", "bitsPerSample").toUInt();
}

// Called to de-serialise a chunk of data in the I/0 device.
void AdapterExample::deserialise(QIODevice* in)
{
    // A pointer to the data blob to fill should be obtained by calling the
    // dataBlob() inherited method. This returns a pointer to an
    // abstract DataBlob, which should be cast to the appropriate type.
    DataBlobExample* blob = (DataBlobExample*) dataBlob();

    // Set the size of the data blob to fill.
    // The chunk size is obtained by calling the chunkSize() inherited method.
    size_t nBytes = chunkSize();
    unsigned length = nBytes / _nBitsPerSample;
    blob->resize(length);

    // Create a temporary buffer for storing the chunk.
    std::vector<char> byteArray(nBytes);

    // Read the chunk from the I/O device.
    in->read(&byteArray[0], nBytes);

    // Get the pointer to the data array in the data blob being filled.
    float* data = blob->data();

    // Fill the data blob.
    for (unsigned i = 0; i < length; ++i) {
        data[i] = *reinterpret_cast<float*>(&byteArray[i * sizeof(float)]);
    }
}
