#include "ServiceDataRequest.h"


#include "utility/memCheck.h"

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

QString ServiceDataRequest::version(const QString& version) const
{
    return _dataRequested.value(version);
}

} // namespace pelican
