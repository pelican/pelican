#include "pelican/comms/StreamData.h"
#include "pelican/comms/DataChunk.h"
#include <iostream>

namespace pelican {

// class StreamData
StreamData::StreamData(const QString& name, void* data, size_t size)
    : DataChunk(name, data, size)
{
}

StreamData::StreamData(const QString& name, QString& id, QByteArray& d )
    : DataChunk(name, id, d) 
{
}

StreamData::StreamData(const QString& name, QString& id, size_t size)
    : DataChunk(name, id, size)
{
}

StreamData::~StreamData()
{
}

const StreamData::DataList_t& StreamData::associateData() const
{
    return _associateData;
}

const QSet<QString>& StreamData::associateDataTypes() const
{
    return _associateDataTypes;
}

void StreamData::addAssociatedData(boost::shared_ptr<DataChunk> data)
{
    Q_ASSERT( data.get() );
    _associateData.append(data);
    _associateDataTypes.insert(data->name());
}

bool StreamData::isValid() const
{
    bool rv = DataChunk::isValid();
    if( rv ) {
        foreach(const boost::shared_ptr<DataChunk>& d, _associateData) {
            rv &= d->isValid();
        }
    }
    return rv;
}

bool StreamData::operator==(const StreamData& sd) const
{
    if( _associateData.size() != sd._associateData.size() )
        return false;
    bool rv = DataChunk::operator==(sd);
    for( int i = 0; i < _associateData.size(); ++i )
    {
        rv &= _associateData[i]->operator==(*(sd._associateData[i]));
    }
    return rv;
}


void StreamData::reset( size_t size )
{
    _associateData.clear();
    setSize(size);
}

} // namespace pelican
