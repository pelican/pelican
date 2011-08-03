#include "pelican/comms/StreamDataRequest.h"

#include "pelican/data/DataRequirements.h"

namespace pelican {


// class StreamDataRequest
StreamDataRequest::StreamDataRequest()
    : ServerRequest(ServerRequest::StreamData)
{
    _dataOptions.clear();
    _dataOptions.end();
}

StreamDataRequest::~StreamDataRequest()
{
}

void StreamDataRequest::addDataOption(const DataRequirements& data)
{
    if( data.streamData().size() != 0 )
        _dataOptions.append(data);
}

bool StreamDataRequest::operator==(const ServerRequest& req) const
{
    bool r = ServerRequest::operator==(req);
    if( r ) {
        const StreamDataRequest& sr = static_cast<const StreamDataRequest&>(req);
        return _dataOptions == sr._dataOptions;
    }
    return r;
}

} // namespace pelican
