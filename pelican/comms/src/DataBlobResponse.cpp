#include "DataBlobResponse.h"


#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details DataBlobResponse 
 */
DataBlobResponse::DataBlobResponse( const QString& blobType, const QString& streamName )
    : ServerResponse( ServerResponse::Blob ), _type(blobType), _name(streamName)
{
}

/**
 *@details
 */
DataBlobResponse::~DataBlobResponse()
{
}

} // namespace pelican
