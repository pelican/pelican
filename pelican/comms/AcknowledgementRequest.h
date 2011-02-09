#ifndef ACKNOWLEDGEMENTREQUEST_H
#define ACKNOWLEDGEMENTREQUEST_H

/**
 * @file AcknowledgementRequest.h
 */

#include "ServerRequest.h"

namespace pelican {

/**
 * @ingroup c_comms
 *
 * @class AcknowledgementRequest
 *
 * @brief
 * Request for an acknowledgement from the server.
 *
 * @details
 *
 */
class AcknowledgementRequest : public ServerRequest
{
    public:
        /// Creates the AcknowledgementRequest object.
        AcknowledgementRequest() : ServerRequest(ServerRequest::Acknowledge) {}

        /// Destroys the AcknowledgementRequest object.
        ~AcknowledgementRequest() {}
};

} // namespace pelican

#endif // ACKNOWLEDGEMENTREQUEST_H
