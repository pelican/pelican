#include "Data.h"
#include "utility/memCheck.h"

namespace pelican {


// class Data 
Data::Data(char* data, size_t size)
    : _data(data), _size(size)
{
}

Data::~Data()
{
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
