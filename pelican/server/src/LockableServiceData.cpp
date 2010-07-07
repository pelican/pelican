#include "pelican/server/LockableServiceData.h"
#include "pelican/comms/Data.h"
#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs a new LockableData object.
 */
LockableServiceData::LockableServiceData(const QString& name, void* data, size_t size,
        QObject* parent) : AbstractLockableData(size, parent)
{
    _data.reset( new Data(name, data, size) );
}

} // namespace pelican
