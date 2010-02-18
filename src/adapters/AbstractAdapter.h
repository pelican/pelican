#ifndef ABSTRACTADAPTER_H
#define ABSTRACTADAPTER_H

#include "utility/ConfigNode.h"
//#include <QDataStream>
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
 * de-serialise data from an input stream.
 *
 * Inherit this class and implement the stream operator method to create a new
 * adapter.
 */

class AbstractAdapter
{
    public:
        /// Constructs a new adapter.
        AbstractAdapter(const ConfigNode& config);

        /// Destroys the adapter (virtual).
        virtual ~AbstractAdapter();

    public:
        virtual void deserialise(QIODevice* in) = 0;

    protected:
        /// Data blob pointer to be by the adapter.
        DataBlob* _data;

        /// Chunk size in bytes.
        std::size_t _chunkSize;
};

} // namespace pelican

#endif // ABSTRACTADAPTER_H
