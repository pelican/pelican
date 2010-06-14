#ifndef ADAPTERREALDATA_H
#define ADAPTERREALDATA_H

#include "pelican/core/AbstractStreamAdapter.h"
#include "pelican/utility/constants.h"

/**
 * @file AdapterRealData.h
 */

namespace pelican {

class ConfigNode;

/**
 * @class AdapterRealData
 *
 * @brief
 * Adapter to deserialise a chunk of data into a PELICAN real-valued data blob.
 *
 * @details
 * Stream adapter to deserialise a chunk of data into a PELICAN real-valued
 * data blob. The initial size of the visibility data blob is set from the
 * configuration options.
 */
class AdapterRealData : public AbstractStreamAdapter
{
        friend class AdapterRealDataTest;

    public:
        /// Constructs the adapter
        AdapterRealData(const ConfigNode& config);

    protected:
        /// Method to deserialise memory held by an I/O device.
        void deserialise(QIODevice* in);
};

} // namespace pelican

#endif // ADAPTERREALDATA_H
