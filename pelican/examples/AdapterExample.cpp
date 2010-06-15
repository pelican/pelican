#include "AdapterExample.h"
#include "pelican/examples/DataBlobExample.h"


// Register the adapter with the factory.
PELICAN_DECLARE_ADAPTER(AdapterExample)

/**
 * @details AdapterExample
 * You will be passed a config node in the constructor
 * to help set up the adapter.
 * You can specify whatever requirements you want in your
 * configuration.
 */


// Construct the example adapter.
AdapterExample::AdapterExample(const ConfigNode& config)
    : AbstractStreamAdapter(config)
{
    // Read the configuration using configuration node utility methods.
    _nSamples = config.getOption("samples", "number").toUInt();
    _nBitsPerSample = config.getOption("samples", "bitsPerSample").toUInt();
}


// Called to de-serialise a chunk of data in the I/0 device.
void AdapterExample::deserialise(QIODevice* in)
{
    // Set the size of the data blob to fill. Note that the pointer to the
    // blob being filled (_data) is a pointer to an abstract data blob in the
    // adapter base class.
    DataBlobExample* blob = static_cast<DataBlobExample*>(_data);
    unsigned length = _chunkSize / _nBitsPerSample;
    blob->resize(length);

    // Create a temporary buffer for storing the chunk.
    std::vector<char> temp(_chunkSize);

    // Read the chunk from the I/O device.
    in->read(&temp[0], _chunkSize);

    // Get the pointer to the data array in the data blob being filled.
    float* data = blob->ptr();

    // Fill the data blob.
    for (unsigned i = 0; i < length; ++i) {
        data[i] = *reinterpret_cast<float*>(temp[i]);
    }
}
