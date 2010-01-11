#include "StreamData.h"
#include "LockedData.h"

#include "utility/memCheck.h"

namespace pelican {


// class StreamData 
StreamData::StreamData(void* data, size_t size)
    : Data(data,size)
{
}

StreamData::~StreamData()
{
}

StreamData::DataList_t& StreamData::associateData()
{
    return _serviceData;
}

void StreamData::addAssociatedData(Data* data)
{
    _serviceData.append(LockedData(data));
}

void StreamData::addAssociatedData(const LockedData& data)
{
    _serviceData.append(data);
}

void StreamData::reset()
{
    _serviceData.clear();
}

bool StreamData::isValid() const
{
    bool rv = Data::isValid();
    foreach(LockedData data, _serviceData )
    {
        rv = rv && data.isValid();
    } 
    return rv;
}

} // namespace pelican
