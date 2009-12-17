#ifndef SERVICEDATAREQUEST_H
#define SERVICEDATAREQUEST_H


#include "ServerRequest.h"

/**
 * @file ServiceDataRequest.h
 */

namespace pelican {

/**
 * @class ServiceDataRequest
 *  
 * @brief
 *   A request for a specific verson of service data
 * @details
 * 
 */

class ServiceDataRequest : public ServerRequest
{
    public:
        ServiceDataRequest(  );
        ~ServiceDataRequest();

    private:
};

} // namespace pelican
#endif // SERVICEDATAREQUEST_H 
