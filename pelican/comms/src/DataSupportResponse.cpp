#include "DataSupportResponse.h"


namespace pelican {


/**
 *@details DataSupportResponse 
 */
DataSupportResponse::DataSupportResponse( const QSet<QString>& streams, const QSet<QString>& serviceStreams )
    : ServerResponse(ServerResponse::DataSupport)
      , _streams(streams), _services(serviceStreams)
{
}

/**
 *@details
 */
DataSupportResponse::~DataSupportResponse()
{
}

} // namespace pelican
