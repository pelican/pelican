#include "LockableStreamData.h"
#include "LockableData.h"
#include "LockedData.h"
#include "comms/Data.h"
#include "comms/StreamData.h"
#include "utility/memCheck.h"

namespace pelican {

// class LockableStreamData 
LockableStreamData::LockableStreamData(const QString& name, void* data, size_t size, QObject* parent)
    : AbstractLockableData(parent)
{
    _data = new StreamData(name,data,size);
}

LockableStreamData::~LockableStreamData()
{
}

const LockableStreamData::DataList_t& LockableStreamData::associateData() const
{
    return _serviceData;
}

const QSet<QString>& LockableStreamData::associateDataTypes() const
{
    return _serviceDataTypes;
}

void LockableStreamData::addAssociatedData(const LockedData& data)
{
    _serviceDataTypes.insert(data.name());
    _serviceData.append(data);
}

void LockableStreamData::reset()
{
    _serviceData.clear();
}

bool LockableStreamData::isValid() const
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

bool LockableStreamData::isValid(const QSet<QString>& associates) const
{
    Q_ASSERT( data() != 0 );
    bool rv = data()->isValid();
    foreach(const QString& assoc, associates )
    {
        foreach(LockedData data, _serviceData ) 
        {
            if( data.name() == assoc ) 
                rv = rv && data.isValid();
        }
    }
    return rv;
}

StreamData* LockableStreamData::streamData() const { 
    return static_cast<StreamData*>(_data); 
}

} // namespace pelican
