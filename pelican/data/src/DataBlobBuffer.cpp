#include "DataBlobBuffer.h"
#include "pelican/data/DataBlob.h"
#include <iostream>

namespace pelican {

/**
 *@details DataBlobBuffer 
 */
DataBlobBuffer::DataBlobBuffer()
        : _index(-1)
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
    return _data[++_index%_size];
}

} // namespace pelican
