#include "pelican/server/LockableData.h"
#include "pelican/comms/Data.h"
#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs a new LockableData object.
 */
LockableData::LockableData(const QString& name, void* data, size_t size,
        QObject* parent) : AbstractLockableData(size, parent)
{
    _data.reset( new Data(name, data, size) );
}

} // namespace pelican
