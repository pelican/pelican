#include "StreamData.h"


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

} // namespace pelican
