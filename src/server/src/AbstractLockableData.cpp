#include "AbstractLockableData.h"
#include "comms/Data.h"


#include "utility/memCheck.h"

namespace pelican {

/**
 *@details AbstractLockableData 
 */
AbstractLockableData::AbstractLockableData(quint64 maxSize, QObject* parent)
    : AbstractLockable(parent), _data(0)
{
    _maxSize = maxSize;
}

/**
 *@details
 */
AbstractLockableData::~AbstractLockableData()
{
    delete _data;
}

void AbstractLockableData::setId(const QString& id)
{
    _data->setId(id);
}

QString AbstractLockableData::id() const { 
    return _data->id(); 
}
} // namespace pelican
