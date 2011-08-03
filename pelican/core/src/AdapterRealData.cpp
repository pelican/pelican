#include "pelican/core/AdapterRealData.h"
#include "pelican/data/ArrayData.h"
#include "pelican/utility/ConfigNode.h"
#include <QtCore/QIODevice>

namespace pelican {

/**
 * @details
 * Constructs a stream adapter for a real-valued data set.
 * Extracts the data blob dimensions from the configuration node,
 * setting some defaults if configuration options can't be found.
 *
 * @param[in]   config  Pelican XML configuration node object.
 */
AdapterRealData::AdapterRealData(const ConfigNode& config)
: AbstractStreamAdapter(config)
{
}

/**
 * @details
 * Method to deserialise a real-valued data chunk
 * contained in a QIODevice into a Pelican real-valued data blob.
 *
 * @param[in] in  Pointer to a QIODevice containing serialised data.
 */
void AdapterRealData::deserialise(QIODevice* in)
{
    // Set the size of the data blob to fill.
    DoubleData* blob = static_cast<DoubleData*>(_data);
    unsigned length = _chunkSize / sizeof(double);
    if (_chunkSize % sizeof(double) != 0) length++;
    blob->resize(length);

    // Read the data off the device.
    in->read((char*)blob->ptr(), _chunkSize);
}

} // namespace pelican
