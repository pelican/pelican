#include "WritableData.h"
#include "Data.h"


#include "utility/memCheck.h"

namespace pelican {

// class WritableData 
WritableData::WritableData(Data* data)
    : _data(data)
{
    if( _data )
        _data->lock();
}

WritableData::~WritableData()
{
    if( _data )
        _data->unlock();
}

Data* WritableData::data()
{
    return _data;
}

} // namespace pelican
