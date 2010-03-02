#include "Data.h"
#include <iostream>
#include "utility/memCheck.h"

namespace pelican {


// class Data 
Data::Data(const QString& name, void* data, size_t size)
    : _name(name), _data(data), _size(size)
{
}

Data::~Data()
{
}

bool Data::isValid() const 
{
    return !( _data == 0 || _size == 0);
}

void* Data::operator*()
{
    return _data;
}

void Data::setSize(size_t s)
{
    _size = s;
}

void Data::setName(const QString& name )
{
    _name = name;
}

size_t Data::size() const
{
    return _size;
}

} // namespace pelican
