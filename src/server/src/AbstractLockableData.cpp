#include "AbstractLockableData.h"
#include "comms/Data.h"


#include "utility/memCheck.h"

namespace pelican {

/**
 *@details AbstractLockableData 
 */
AbstractLockableData::AbstractLockableData(QObject* parent)
    : AbstractLockable(parent), _data(0)
{
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
