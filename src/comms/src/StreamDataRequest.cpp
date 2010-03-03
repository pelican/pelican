#include "StreamDataRequest.h"

#include "data/DataRequirements.h"
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

void StreamDataRequest::addDataOption(const QList<DataRequirements>& data)
{
    _dataOptions += QVector<DataRequirements>::fromList(data);
}

StreamDataRequest::DataRequirementsIterator StreamDataRequest::begin() const
{
    return _dataOptions.begin();
}

StreamDataRequest::DataRequirementsIterator StreamDataRequest::end() const
{
    return _dataOptions.end();
}

int StreamDataRequest::size() const
{
    return _dataOptions.size();
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
