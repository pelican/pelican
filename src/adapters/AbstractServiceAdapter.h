#ifndef ABSTRACTSERVICEADAPTER_H
#define ABSTRACTSERVICEADAPTER_H

#include "adapters/AbstractAdapter.h"

/**
 * @file AbstractServiceAdapter.h
 */

namespace pelican {

class ConfigNode;
class DataBlob;

/**
 * @class AbstractServiceAdapter
 *  
 * @brief
 * Abstract base class for all service data adapters.
 *
 * @details
 * This class is the base class for all Pelican service data adapters, which
 * de-serialise data from an input stream.
 *
 * Inherit this class and implement the stream operator method to create a new
 * adapter.
 */

class AbstractServiceAdapter : public AbstractAdapter
{
    public:
        /// Constructs a new adapter.
        AbstractServiceAdapter(const ConfigNode& config);

        /// Destroys the adapter (virtual).
        virtual ~AbstractServiceAdapter();

    public:
        /// Configures an abstract adapter
        AbstractServiceAdapter& config(DataBlob* data, std::size_t size);

};

} // namespace pelican

#endif // ABSTRACTSERVICEADAPTER_H
