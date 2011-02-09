#ifndef DATASUPPORTREQUEST_H
#define DATASUPPORTREQUEST_H

/**
 * @file DataSupportRequest.h
 */

#include "ServerRequest.h"

namespace pelican {

/**
 * @ingroup c_comms
 *
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
