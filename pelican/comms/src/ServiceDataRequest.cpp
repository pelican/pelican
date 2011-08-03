#include "pelican/comms/ServiceDataRequest.h"

namespace pelican {


// class ServiceDataRequest
ServiceDataRequest::ServiceDataRequest()
    : ServerRequest(ServerRequest::ServiceData)
{
}

ServiceDataRequest::~ServiceDataRequest()
{
}

void ServiceDataRequest::request(const QString& type, const QString& version)
{
    _dataRequested[type] = version;
}

QList<QString> ServiceDataRequest::types() const
{
    return _dataRequested.keys();
}

QString ServiceDataRequest::version(const QString& type) const
{
    return _dataRequested.value(type);
}

bool ServiceDataRequest::isEmpty() const
{
    return _dataRequested.isEmpty();
}

bool ServiceDataRequest::operator==(const ServiceDataRequest& req) const
{
    return _dataRequested == req._dataRequested;
}

bool ServiceDataRequest::operator==(const ServerRequest& request) const
{
    bool r = ServerRequest::operator==(request);
    if( r ) {
        r = operator==( static_cast<const ServiceDataRequest& >( request ) );
    }
    return r;
}

} // namespace pelican
