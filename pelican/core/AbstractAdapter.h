#ifndef ABSTRACTADAPTER_H
#define ABSTRACTADAPTER_H

/**
 * @file AbstractAdapter.h
 */

#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/FactoryRegistrar.h"

class QIODevice;

namespace pelican {


// This macro is used to register the named adapter type.
#define PELICAN_DECLARE_ADAPTER(type) PELICAN_DECLARE(AbstractAdapter, type)

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
 * When creating a new adapter, do not inherit this class directly, but use
 * either the AbstractStreamAdapter or AbstractServiceAdapter classes for
 * (respectively) stream data or service data. In either case, the
 * deserialise() method must be implemented.
 */

class AbstractAdapter
{
    public:
        typedef enum { Stream, Service } AdapterType_t;

    public:
        PELICAN_CONSTRUCT_TYPES(ConfigNode)

        /// Constructs an adapter of the given type and configuration.
        AbstractAdapter(AdapterType_t type, const ConfigNode&)
        : _type(type), _chunkSize(0), _data(0) {}

        /// Destroys the adapter (virtual).
        virtual ~AbstractAdapter() {}

        /// Return the type of adapter as passed in the constructor.
        AdapterType_t type() const { return _type; }

        /// Deserialises the data from the input device.
        virtual void deserialise(QIODevice* in) = 0;

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
};

} // namespace pelican

#endif // ABSTRACTADAPTER_H
