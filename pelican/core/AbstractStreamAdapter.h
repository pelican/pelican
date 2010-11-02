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
 *
 * Inherit this class and implement the stream operator method to create a new
 * adapter.
 */
class AbstractStreamAdapter : public AbstractAdapter
{
    public:
        /// Constructs a new stream adapter with the given configuration.
        AbstractStreamAdapter(const ConfigNode& config)
        : AbstractAdapter(AbstractAdapter::Stream, config) {}

        /// Destroys the stream adapter (virtual).
        virtual ~AbstractStreamAdapter() {}

    public:
        /// Configures the abstract stream adapter.
        void config(DataBlob* data, std::size_t chunkSize,
                const QHash<QString, DataBlob*>& serviceData) {
            _data = data;
            _chunkSize = chunkSize;
            _serviceData = serviceData;
        }

    protected:
        /// Hash of service data blobs associated with the stream data.
        /// e.g. Service data containing updated dimensions of the stream data.
        QHash<QString, DataBlob*> _serviceData;
};

} // namespace pelican

#endif // ABSTRACTSTREAMADAPTER_H
