#include "AcknowledgementRequest.h"


#include "utility/memCheck.h"

namespace pelican {


// class AcknowledgementRequest 
AcknowledgementRequest::AcknowledgementRequest()
    : ServerRequest(ServerRequest::Acknowledge)
{
}

AcknowledgementRequest::~AcknowledgementRequest()
{
}

} // namespace pelican
