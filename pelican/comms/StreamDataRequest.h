#ifndef STREAMDATAREQUEST_H
#define STREAMDATAREQUEST_H

#include <QVector>
#include "ServerRequest.h"
#include "data/DataRequirements.h"
#include <iostream>

/**
 * @file StreamDataRequest.h
 */

namespace pelican {

/**
 * @class StreamDataRequest
 *
 * @brief
 * Specifications of a set of DataRequirements.
 *
 * @details
 *
 */
class StreamDataRequest : public ServerRequest
{
    private:
        QVector<DataRequirements> _dataOptions;

    public:
        typedef QVector<DataRequirements>::const_iterator DataRequirementsIterator;

    public:
        StreamDataRequest();
        ~StreamDataRequest();

        /// Adds a dataset specification that is required.
        /// The first added option will be serviced if available
        /// otherwise the server will try to satisfy subsequent options
        void addDataOption(const DataRequirements& data);

        /// Returns an iterator for the DataRequirements options.
        /// The order will correspond to the addDataOption call sequence.
        DataRequirementsIterator begin() const {return _dataOptions.begin();}

        /// Returns an iterator at the end of the DataRequirements options.
        DataRequirementsIterator end() const {return _dataOptions.end();}

        /// Returns true if there are no stream data
        /// in any of the requirements passed.
        bool isEmpty() const { return _dataOptions.empty(); }

        /// The number of requirements.
        int size() const {return _dataOptions.size();}

        /// Test for equality between ServiceData objects.
        virtual bool operator==(const ServerRequest&) const;
};

typedef StreamDataRequest::DataRequirementsIterator DataRequirementsIterator;

} // namespace pelican

#endif // STREAMDATAREQUEST_H 
