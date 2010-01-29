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
 *    Request for an Acknowledgement from the server
 * @details
 * 
 */

class AcknowledgementRequest : public ServerRequest
{
    public:
        AcknowledgementRequest(  );
        ~AcknowledgementRequest();

    private:
};

} // namespace pelican
#endif // ACKNOWLEDGEMENTREQUEST_H 
