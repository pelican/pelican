#ifndef STREAMDATARESPONSE_H
#define STREAMDATARESPONSE_H


#include "ServerResponse.h"

/**
 * @file StreamDataResponse.h
 */

namespace pelican {

/**
 * @class StreamDataResponse
 *  
 * @brief
 *    class to represent a data stream returned from the server
 * @details
 * 
 */

class StreamDataResponse : public ServerResponse
{
    public:
        StreamDataResponse();
        ~StreamDataResponse();

    private:
};

} // namespace pelican
#endif // STREAMDATARESPONSE_H 
