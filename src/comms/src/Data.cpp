#include "Data.h"
#include <iostream>
#include "utility/memCheck.h"

namespace pelican {


// class Data 
Data::Data(const QString& name, void* data, size_t size)
    : _name(name), _data(data), _size(size)
{
}

Data::Data(const QString& name, const QString& id, size_t size)
    : _name(name), _id(id), _data(0), _size(size)
{
}

Data::Data(const QString& name, const QString& id, QByteArray& ba )
    : _name(name), _id(id)
{
    _data = ba.data();
    _size = ba.size();
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

bool Data::operator==(const Data& d) const
{
    bool rv = _name == d.name() && _size == d._size && _id == d._id;
    //std::cout << "name=" << _name.toStdString() << " id=" << _id.toStdString()<< " size=" << _size << std::endl;
    //std::cout << "name=" << d._name.toStdString() << " id=" << d._id.toStdString()<< " size=" << d._size << std::endl;
    return rv;
}

size_t Data::size() const
{
    return _size;
}

} // namespace pelican
