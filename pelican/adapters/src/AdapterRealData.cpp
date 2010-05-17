#include "pelican/adapters/AdapterRealData.h"
#include "pelican/data/RealData.h"
#include "pelican/utility/ConfigNode.h"
#include <QIODevice>

#include "pelican/utility/memCheck.h"

namespace pelican {

PELICAN_DECLARE_ADAPTER(AdapterRealData)

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
    RealData* blob = static_cast<RealData*>(_data);
    int length = _chunkSize / sizeof(real_t);
    if (_chunkSize % sizeof(real_t) != 0) length++;
    blob->resize(length);

    // Read the data off the device.
    in->read((char*)blob->ptr(), _chunkSize);
}

} // namespace pelican