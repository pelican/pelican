#include "ServiceDataResponse.h"

#include "Data.h"
#include "utility/memCheck.h"

namespace pelican {


// class ServiceDataResponse 
ServiceDataResponse::ServiceDataResponse()
    : ServerResponse(ServerResponse::ServiceData)
{
}

ServiceDataResponse::~ServiceDataResponse()
{
    foreach(Data* d, _data )
        delete d;
}

void ServiceDataResponse::addData(Data* d)
{
    _data.append(d);
}

const QList<Data*>& ServiceDataResponse::data() const
{
    return _data;
}


} // namespace pelican
