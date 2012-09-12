#include "DataSupportResponse.h"


namespace pelican {


/**
 *@details DataSupportResponse 
 */
DataSupportResponse::DataSupportResponse( const DataSpec& streams )
    : ServerResponse(ServerResponse::DataSupport) , _spec(streams)
{
}

DataSupportResponse::DataSupportResponse(const QSet<QString>& streams, const QSet<QString>& service  )
    : ServerResponse(ServerResponse::DataSupport)
{
    _spec.addStreamData( streams );
    _spec.addServiceData( service );
}
/**
 *@details
 */
DataSupportResponse::~DataSupportResponse()
{
}

} // namespace pelican
