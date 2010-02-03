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
 *   Specifications of a set of DataRequirements
 * @details
 *
 */

class StreamDataRequest : public ServerRequest
{
    public:
        typedef QVector<DataRequirements>::const_iterator DataRequirementsIterator;

    public:
        StreamDataRequest();
        ~StreamDataRequest();
        /// addDataOption
        //  Adds a dataset specification that is required.
        //  The first added option will be serviced if available
        //  otherwise the server will try to satisfy subsequent options
        void addDataOption(const DataRequirements& data);

        /// begin
        //  returns an iterator for the DataRequirements options
        //  The order will correspond to the addDataOption call 
        //  sequence
        DataRequirementsIterator begin() const;

        /// end
        //  returns an iterator at the end of the
        //  DataRequirements options
        DataRequirementsIterator end() const;

        int size() const;

        // test for equality between ServiceData objects
        virtual bool operator==(const ServerRequest&) const;

    private:
        QVector<DataRequirements> _dataOptions;
};

typedef StreamDataRequest::DataRequirementsIterator DataRequirementsIterator;

} // namespace pelican
#endif // STREAMDATAREQUEST_H 
