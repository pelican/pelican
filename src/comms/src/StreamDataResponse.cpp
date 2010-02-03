#include "StreamDataResponse.h"


#include "utility/memCheck.h"

namespace pelican {


// class StreamDataResponse 
StreamDataResponse::StreamDataResponse()
    : ServerResponse( ServerResponse::StreamData )
{
}

StreamDataResponse::~StreamDataResponse()
{
}

} // namespace pelican
