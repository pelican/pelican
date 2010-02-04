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

} // namespace pelican
