#ifndef ADAPTER_EXAMPLE_H
#define ADAPTER_EXAMPLE_H

#include "pelican/adapters/AbstractStreamAdapter.h"

using namespace pelican;

/**
 * @class AdapterExample
 *
 * @brief
 *    An example of a Stream Adapter class
 */

class AdapterExample : public AbstractStreamAdapter
{
    public:
        AdapterExample(const ConfigNode& config);

    protected:
        /// Method to deserialise chunks of memory provided by the I/O device.
        void deserialise(QIODevice* in);

    private:
        unsigned _nSamples;
        unsigned _nBitsPerSample;
};

#endif // ADAPTER_EXAMPLE_H
