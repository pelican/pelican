#ifndef ABSTRACTSTREAMADAPTER_H
#define ABSTRACTSTREAMADAPTER_H

#include "adapters/AbstractAdapter.h"
#include <QHash>
#include <QString>
#include <QIODevice>

/**
 * @file AbstractStreamAdapter.h
 */

namespace pelican {

class ConfigNode;
class DataBlob;

/**
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
        /// Constructs a new adapter.
        AbstractStreamAdapter(const ConfigNode& config);

        /// Destroys the adapter (virtual).
        virtual ~AbstractStreamAdapter();

    public:
        /// Configures an abstract adapter for use with the stream operator
        AbstractStreamAdapter& config(DataBlob* data,
                std::size_t size, const QHash<QString, DataBlob*>& serviceData);

    protected:
        /// Hash of service data blobs associated adaption of the stream data
        /// e.g. Service data containing updated dimensions of the stream data.
        QHash<QString, DataBlob*> _serviceData;
};

} // namespace pelican

#endif // ABSTRACTSTREAMADAPTER_H
