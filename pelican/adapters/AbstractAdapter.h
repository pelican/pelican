#ifndef ABSTRACTADAPTER_H
#define ABSTRACTADAPTER_H

#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/Factory.h"
#include <QIODevice>
#include <iostream>

/**
 * @file AbstractAdapter.h
 */

namespace pelican {

/**
 * This macro is used to register the named adapter type with the factory.
 * It should be used within the global scope of the adapter's source file.
 *
 * @note
 * The macro expands to declare a dummy variable of the object's generator
 * type, which (when constructed), adds the type name to the creator's known
 * object types.
 *
 * The macro is placed within the global scope so that it is initialised as
 * soon as the program starts executing. It is placed within an anonymous
 * namespace so that the dummy creator variable is not accessible from outside
 * the file that instantiated it.
 */
#define PELICAN_DECLARE_ADAPTER(type) namespace {Creator<type, AbstractAdapter> reg(#type);}

class DataBlob;

/**
 * @class AbstractAdapter
 *
 * @brief
 * Abstract base class for all data adapters.
 *
 * @details
 * This class is the base class for all Pelican data adapters, which
 * de-serialise data from an input device.
 *
 * Inherit this class and implement the deserialise method to create a new
 * adapter.
 */
class AbstractAdapter
{
    public:
        typedef enum { Stream, Service } AdapterType_t;

    protected:
        AdapterType_t _type;    ///< Type of adapter.
        std::size_t _chunkSize; ///< Chunk size in bytes.
        DataBlob* _data;        ///< Pointer to data blob to be filled.

    public:
        /// Constructs an adapter of the given type and configuration.
        AbstractAdapter(AdapterType_t type, const ConfigNode&)
        : _type(type), _chunkSize(0), _data(0) {}

        /// Destroys the adapter (virtual).
        virtual ~AbstractAdapter() {}

        /// Return the type of adapter as passed in the constructor.
        AdapterType_t type() const {return _type;}

        /// Deserialises the data from the input device.
        virtual void deserialise(QIODevice* in) = 0;
};

} // namespace pelican

#endif // ABSTRACTADAPTER_H
