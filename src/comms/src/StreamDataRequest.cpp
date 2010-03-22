#include "StreamDataRequest.h"

#include "data/DataRequirements.h"
#include "utility/memCheck.h"

namespace pelican {


// class StreamDataRequest 
StreamDataRequest::StreamDataRequest()
    : ServerRequest(ServerRequest::StreamData)
{
    std::cout << "Constructor" << std::endl;
    _dataOptions.clear();
    _dataOptions.end();
    std::cout << "Constructor Done" << std::endl;
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
