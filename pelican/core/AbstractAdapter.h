#ifndef ABSTRACTADAPTER_H
#define ABSTRACTADAPTER_H

/**
 * @file AbstractAdapter.h
 */

#include "utility/ConfigNode.h"
#include "utility/FactoryRegistrar.h"

class QIODevice;

// This macro is used to register the named adapter type.
#define PELICAN_DECLARE_ADAPTER(type) PELICAN_DECLARE(pelican::AbstractAdapter, type)

namespace pelican {

class DataBlob;
class ConfigNode;

/**
 * @ingroup c_core
 *
 * @class AbstractAdapter
 *
 * @brief
 * Abstract base class for all data adapters.
 *
 * @details
 * This class is the base class for all Pelican data adapters, which
 * de-serialise data from an input device.
 *
 * You would normally inherit directly from this class using the default
 * Unknown type to allow the server configuration to determine the type
 * of adapter it is at runtime.
 *
 * The deserialise() method must be implemented.
 */

class AbstractAdapter
{
    public:
        // Unknown implies that the type will be determined
        // at runtime by the configuration of the DataManager
        typedef enum { Unknown, Stream, Service } AdapterType_t;

    public:
        PELICAN_CONSTRUCT_TYPES(ConfigNode)

        /// Constructs an adapter of the given type and configuration.
        AbstractAdapter(const ConfigNode&, AdapterType_t type = Unknown)
        : _type(type), _chunkSize(0), _data(0) {}

        /// Destroys the adapter (virtual).
        virtual ~AbstractAdapter() {}

        /// Return the type of adapter as passed in the constructor.
        AdapterType_t type() const { return _type; }

        /// set the type of adapter .
        void setType( const AdapterType_t& type ) { _type = type; }

        /// Deserialises the data from the input device.
        virtual void deserialise(QIODevice* in) = 0;

        /// Configures the service adapter, where there are no data dependencies.
        void config(DataBlob* data, std::size_t size) {
            _data = data; _chunkSize = size;
        }

        /// Configures the service adapter, providing the dependent data.
        void config(DataBlob* data, std::size_t chunkSize,
                const QHash<QString, DataBlob*>& serviceData) {
            config(data, chunkSize);
            _serviceData = serviceData;
        }


    protected:
        /// Returns a pointer to the data blob to fill.
        DataBlob* dataBlob() { return _data; }

        /// Returns the chunk size in bytes.
        size_t chunkSize() const { return _chunkSize; }

    protected:
        /// @todo Make these private?
        AdapterType_t _type;    ///< Type of adapter.
        std::size_t _chunkSize; ///< Chunk size in bytes.
        DataBlob* _data;        ///< Pointer to data blob to be filled.
        /// Hash of service data blobs associated with the stream data.
        /// e.g. Service data containing updated dimensions of the stream data.
        QHash<QString, DataBlob*> _serviceData;
};

} // namespace pelican

#endif // ABSTRACTADAPTER_H
