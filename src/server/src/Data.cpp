#include "Data.h"
#include <QMutexLocker>
#include "utility/memCheck.h"

namespace pelican {


// class Data 
Data::Data(char* data, size_t size)
    : _data(data), _size(size), _lock(0)
{
}

Data::~Data()
{
}

void Data::lock()
{
    QMutexLocker locker(&_mutex);
    ++_lock;
}

void Data::unlock()
{
    QMutexLocker locker(&_mutex);
    --_lock;
}

char* Data::operator*()
{
    return _data;
}

size_t Data::size() const
{
    return _size;
}

} // namespace pelican
