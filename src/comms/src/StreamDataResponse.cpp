#include "StreamDataResponse.h"


#include "utility/memCheck.h"

namespace pelican {


// class StreamDataResponse 
StreamDataResponse::StreamDataResponse()
    : ServerResponse( ServerResponse::StreamData )
{
    _data = 0;
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
