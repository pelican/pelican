#ifndef ABSTRACTADAPTER_H
#define ABSTRACTADAPTER_H

#include "data/DataBlob.h"
#include "utility/ConfigNode.h"
#include <QString>
#include <QDataStream>

/**
 * @file AbstractAdapter.h
 */

namespace pelican {

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
    private:
        /// The configuration node for the adapter.
        const ConfigNode *_config;

    public:
        /// Constructs a new adapter.
        AbstractAdapter(const ConfigNode& config);

        /// Destroys the adapter (virtual).
        virtual ~AbstractAdapter();

    public:
        /// Adapts data from an input stream (pure virtual).
        virtual void run(QDataStream& stream, const size_t& size) = 0;
};

} // namespace pelican

#endif // ABSTRACTADAPTER_H
