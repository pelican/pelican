#include "DataBlobBuffer.h"
#include "pelican/data/DataBlob.h"

namespace pelican {

/**
 *@details DataBlobBuffer 
 */
DataBlobBuffer::DataBlobBuffer()
        : _index(0)
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
}

DataBlob* DataBlobBuffer::next() {
    return _data[++_index%_data.size()];
}

} // namespace pelican
