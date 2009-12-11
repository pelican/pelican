#ifndef STREAMDATAREQUEST_H
#define STREAMDATAREQUEST_H


#include <QVector>
#include "ServerRequest.h"

/**
 * @file StreamDataRequest.h
 */

namespace pelican {

class DataRequirements;

/**
 * @class StreamDataRequest
 *  
 * @brief
 *   Contains data relevant to requesting stream data
 * @details
 * 
 */

class StreamDataRequest : public ServerRequest
{
    public:
        StreamDataRequest();
        ~StreamDataRequest();
        /// addDataOption
        //  Adds a dataset specification that is required.
        //  The first added option will be serviced if available
        //  otherwise the server will try to satisfy subsequent options
        void addDataOption(const DataRequirements& data);

    private:
        QVector<DataRequirements> _dataOptions;
};

} // namespace pelican
#endif // STREAMDATAREQUEST_H 
