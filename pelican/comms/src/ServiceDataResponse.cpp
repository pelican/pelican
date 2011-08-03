#include "pelican/comms/ServiceDataResponse.h"

#include "pelican/comms/DataChunk.h"

namespace pelican {

/**
 * @details
 * Constructs a new ServiceDataResponse object.
 */
ServiceDataResponse::ServiceDataResponse()
    : ServerResponse(ServerResponse::ServiceData)
{
}

/**
 * @details
 * Destroys the ServiceDataResponse object.
 *
 * The destructor deletes all the Data objects known to it.
 */
ServiceDataResponse::~ServiceDataResponse()
{
    foreach(DataChunk* d, _data )
        delete d;
}

/**
 * @details
 * Adds a Data object to the list known to this class.
 * This class takes ownership of the Data object and will delete it
 * on destruction.
 */
void ServiceDataResponse::addData(DataChunk* d)
{
    _data.append(d);
}

} // namespace pelican
