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

const StreamData::DataList_t& StreamData::associateData() const
{
    return _serviceData;
}

const QSet<QString>& StreamData::associateDataTypes() const
{
    return _serviceDataTypes;
}

void StreamData::addAssociatedData(const LockedData& data)
{
    _serviceDataTypes.insert(data.name());
    _serviceData.append(data);
}

void StreamData::reset()
{
    _serviceData.clear();
}

bool StreamData::isValid() const
{
    return isValid( _serviceDataTypes );
    /*
    bool rv = Data::isValid();
    foreach(LockedData data, _serviceData )
    {
        rv = rv && data.isValid();
    }
    return rv;
    */
}

bool StreamData::isValid(const QSet<QString>& associates) const
{
    bool rv = Data::isValid();
    foreach(LockedData data, associates )
    {
        rv = rv && data.isValid();
    }
    return rv;
}

} // namespace pelican
