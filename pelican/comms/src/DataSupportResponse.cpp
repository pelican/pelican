#include "DataSupportResponse.h"


#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details DataSupportResponse 
 */
DataSupportResponse::DataSupportResponse( const QSet<QString>& streams )
    : ServerResponse(ServerResponse::DataSupport)
      , _streams(streams)
{
}

/**
 *@details
 */
DataSupportResponse::~DataSupportResponse()
{
}

} // namespace pelican
