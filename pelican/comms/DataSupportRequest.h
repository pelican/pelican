#ifndef DATASUPPORTREQUEST_H
#define DATASUPPORTREQUEST_H


#include "ServerRequest.h"

/**
 * @file DataSupportRequest.h
 */

namespace pelican {

/**
 * @class DataSupportRequest
 *  
 * @brief
 *    Request for Data streams supported by the server
 * @details
 * 
 */

class DataSupportRequest : public ServerRequest
{
    public:
        DataSupportRequest(  );
        ~DataSupportRequest();

    private:
};

} // namespace pelican
#endif // DATASUPPORTREQUEST_H 
