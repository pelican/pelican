#ifndef ABSTRACTADAPTER_H
#define ABSTRACTADAPTER_H

#include "pelican/utility/ConfigNode.h"
#include <QIODevice>
#include <iostream>

/**
 * @file AbstractAdapter.h
 */

namespace pelican {

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
