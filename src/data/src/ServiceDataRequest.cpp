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

} // namespace pelican
