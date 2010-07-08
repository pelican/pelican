#include "DataSupportRequest.h"


#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details DataSupportRequest 
 */
DataSupportRequest::DataSupportRequest()
    : ServerRequest(ServerRequest::DataSupport)
{
}

/**
 *@details
 */
DataSupportRequest::~DataSupportRequest()
{
}

} // namespace pelican
