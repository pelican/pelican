#include "ServiceDataBuffer.h"
#include "LockedData.h"
#include <QMutexLocker>


#include "utility/memCheck.h"

namespace pelican {


// class ServiceDataBuffer 
ServiceDataBuffer::ServiceDataBuffer()
    : DataBuffer()
{
}

ServiceDataBuffer::~ServiceDataBuffer()
{
}

LockedData ServiceDataBuffer::getData(const QString& version)
{
    //QMutexLocker lock(&_mutex);
    return LockedData(_data.value(version)); // returns an invaliid object if it does not exist
}

} // namespace pelican
