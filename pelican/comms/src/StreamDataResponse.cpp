#include "pelican/comms/StreamDataResponse.h"

namespace pelican {

/**
 * @details
 * Creates a new StreamDataResponse object.
 */
StreamDataResponse::StreamDataResponse()
    : ServerResponse( ServerResponse::StreamData ), _data(0)
{
}

/**
 * @details
 * Destroys the StreamDataResponse object, deleting the StreamData
 * object that this class contains.
 */
StreamDataResponse::~StreamDataResponse()
{
    delete _data;
}

} // namespace pelican
