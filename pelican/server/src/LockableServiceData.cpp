#include "pelican/server/LockableServiceData.h"
#include "pelican/comms/DataChunk.h"

namespace pelican {

/**
 * @details
 * Constructs a new LockableData object.
 */
LockableServiceData::LockableServiceData(const QString& name, void* data,
        size_t size, QObject* parent) : AbstractLockableData(size, parent)
{
    _data.reset( new DataChunk(name, data, size) );
}

} // namespace pelican
