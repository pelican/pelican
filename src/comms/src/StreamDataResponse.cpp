#include "StreamDataResponse.h"


#include "utility/memCheck.h"

namespace pelican {


// class StreamDataResponse 
StreamDataResponse::StreamDataResponse()
    : ServerResponse( ServerResponse::StreamData )
{
    _data = 0;
}

void StreamDataResponse::setStreamData( pelican::StreamData* data )
{
    _data = data;
}

StreamDataResponse::~StreamDataResponse()
{
    delete _data;
}

StreamData* StreamDataResponse::streamData()
{
    return _data;
}

} // namespace pelican
