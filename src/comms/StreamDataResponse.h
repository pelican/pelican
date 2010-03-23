#ifndef STREAMDATARESPONSE_H
#define STREAMDATARESPONSE_H

#include "ServerResponse.h"
#include "StreamData.h"

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
        void setStreamData(pelican::StreamData*);
        pelican::StreamData* streamData();

    private:
        pelican::StreamData* _data;
};

} // namespace pelican
#endif // STREAMDATARESPONSE_H
