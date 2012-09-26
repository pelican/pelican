#ifndef ABSTRACTSERVICEADAPTER_H
#define ABSTRACTSERVICEADAPTER_H

/**
 * @file AbstractServiceAdapter.h
 */

#include "pelican/core/AbstractAdapter.h"

namespace pelican {

class ConfigNode;
class DataBlob;

/**
 * @ingroup c_core
 *
 * @class AbstractServiceAdapter
 * @deprecated
 *
 * @brief
 * Abstract base class for all service data adapters.
 *
 * @details
 * This class is the base class for all Pelican service data adapters, which
 * de-serialise data from an input stream.
 *
 * Normally you should inherit directly form AbstractAdapter unless you wish to
 * force pelican to ignore the server configuration and never provide the
 * dependent data
 *
 * Inherit this class and implement the deserialise() method to create a new
 * adapter.
 */
class AbstractServiceAdapter : public AbstractAdapter
{
    public:
        /// Constructs a new service data adapter with the given configuration.
        AbstractServiceAdapter(const ConfigNode& config)
        : AbstractAdapter(config, AbstractAdapter::Service) {}

        /// Destroys the service adapter (virtual).
        virtual ~AbstractServiceAdapter() {}

};

} // namespace pelican

#endif // ABSTRACTSERVICEADAPTER_H
