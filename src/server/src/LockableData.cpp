#include <iostream>
#include "LockableData.h"
#include "comms/Data.h"
#include "utility/memCheck.h"

namespace pelican {

// class LockableData 
LockableData::LockableData(const QString& name, void* data, size_t size, QObject* parent)
    : AbstractLockableData(size, parent)
{
    _data = new Data(name, data, size);
}

LockableData::~LockableData()
{
}

bool LockableData::isValid() const
{
    return _data->isValid();
}

void* LockableData::memoryLocation()
{
    return _data->operator*();
}



QString LockableData::name() const {
    return _data->name();
}

size_t LockableData::size() const {
    return _data->size();
}

} // namespace pelican
