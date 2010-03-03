#include "StreamData.h"
#include "Data.h"

#include "utility/memCheck.h"

namespace pelican {

// class StreamData 
StreamData::StreamData(const QString& name, void* data, size_t size)
    : Data(name, data, size)
{
}

StreamData::StreamData(const QString& name, QString& id, size_t size)
    : Data(name, id, size)
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

void StreamData::addAssociatedData(const Data* data)
{
    _associateData.append(data);
    _associateDataTypes.insert(data->name());
}

bool StreamData::isValid() const
{
    bool rv = Data::isValid();
    if( rv ) {
        foreach(const Data* d, _associateData) {
            rv &= d->isValid();
        }
    }
    return rv;
}

bool StreamData::operator==(const StreamData& sd) const {
    bool rv = Data::operator==(sd);
    rv &= _associateData == sd._associateData;
    return rv;
}


void StreamData::reset()
{
    _associateData.clear();
}

} // namespace pelican
