#ifndef ABSTRACTSTREAMADAPTER_H
#define ABSTRACTSTREAMADAPTER_H

/**
 * @file AbstractStreamAdapter.h
 */

#include "pelican/core/AbstractAdapter.h"
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QIODevice>

namespace pelican {

class ConfigNode;
class DataBlob;

/**
 * @ingroup c_core
 *
 * @class AbstractStreamAdapter
 *
 * @brief
 * Abstract base class for all stream data adapters.
 *
 * @details
 * This class is the base class for all Pelican stream data adapters, which
 * de-serialise data from an input stream.
 
 * Normally you should inherit directly form AbstractAdapter unless you wish to
 * force pelican to ignore the server confiugration and always provide the
 * dependent data
 *
 * Inherit this class and implement the stream operator method to create a new
 * adapter.
 */
class AbstractStreamAdapter : public AbstractAdapter
{
    public:
        /// Constructs a new stream adapter with the given configuration.
        AbstractStreamAdapter(const ConfigNode& config)
        : AbstractAdapter(config, AbstractAdapter::Stream) {}

        /// Destroys the stream adapter (virtual).
        virtual ~AbstractStreamAdapter() {}

};

} // namespace pelican

#endif // ABSTRACTSTREAMADAPTER_H
