#include "StreamDataRequest.h"

#include "DataRequirements.h"
#include "utility/memCheck.h"

namespace pelican {


// class StreamDataRequest 
StreamDataRequest::StreamDataRequest()
    : ServerRequest(ServerRequest::StreamData)
{
}

StreamDataRequest::~StreamDataRequest()
{
}

void StreamDataRequest::addDataOption(const DataRequirements& data)
{
    _dataOptions.append(data);
}

} // namespace pelican
