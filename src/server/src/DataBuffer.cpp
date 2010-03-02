#include "DataBuffer.h"


#include "utility/memCheck.h"

namespace pelican {


// class DataBuffer 
DataBuffer::DataBuffer(const QString& type, QObject* parent)
    : QObject(parent), _type(type)
{
}

DataBuffer::~DataBuffer()
{
}

} // namespace pelican
