#include "DataSupportResponse.h"


#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details DataSupportResponse 
 */
DataSupportResponse::DataSupportResponse()
    : ServerResponse(ServerResponse::DataSupport)
{
}

/**
 *@details
 */
DataSupportResponse::~DataSupportResponse()
{
}

} // namespace pelican
