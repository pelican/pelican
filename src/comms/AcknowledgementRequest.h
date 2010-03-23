#ifndef ACKNOWLEDGEMENTREQUEST_H
#define ACKNOWLEDGEMENTREQUEST_H

#include "ServerRequest.h"

/**
 * @file AcknowledgementRequest.h
 */

namespace pelican {

/**
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
