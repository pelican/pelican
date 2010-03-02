#include "ServiceDataResponse.h"


#include "utility/memCheck.h"

namespace pelican {


// class ServiceDataResponse 
ServiceDataResponse::ServiceDataResponse()
    : ServerResponse(ServerResponse::ServiceData)
{
}

ServiceDataResponse::~ServiceDataResponse()
{
}

const QList<QString>& ServiceDataResponse::types() const
{
    return _dataTypes;
}

long ServiceDataResponse::size(const QString& type) const
{
    return _sizes[type];
}


} // namespace pelican
