#include "DataBlobBuffer.h"
#include "pelican/data/DataBlob.h"
#include <iostream>

namespace pelican {

/**
 *@details DataBlobBuffer 
 */
DataBlobBuffer::DataBlobBuffer()
        : _index(-1), _size(0)
{
}

/**
 *@details
 */
DataBlobBuffer::~DataBlobBuffer()
{
     foreach(DataBlob* blob, _data) {
        delete blob;
     }
}

void DataBlobBuffer::addDataBlob(DataBlob* blob)
{
     _data.append(blob);
     _size = _data.size();
}

DataBlob* DataBlobBuffer::next() {
    _index=++_index%_size;
    return _data[_index];
}

void DataBlobBuffer::shrink(int newSize) {
    // remove oldest/unused data first
    while( _data.size() > newSize ) {
        unsigned int index=_index+1%_size;
        delete _data[index]; 
        _data.removeAt(index);
        if( index < _index && _index != (unsigned int)-1 ) { --_index; }
    }
    _size=_data.size();
}

} // namespace pelican
