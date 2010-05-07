#include "DataBlobResponse.h"


#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details DataBlobResponse 
 */
DataBlobResponse::DataBlobResponse(DataBlob* blob)
    : ServerResponse( ServerResponse::Blob ), _data(blob)
{
}

/**
 *@details
 */
DataBlobResponse::~DataBlobResponse()
{
}

} // namespace pelican
