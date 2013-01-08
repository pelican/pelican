#ifndef STREAMDATAREQUEST_H
#define STREAMDATAREQUEST_H

/**
 * @file StreamDataRequest.h
 */

#include <QtCore/QVector>
#include "comms/ServerRequest.h"
#include "data/DataSpec.h"
#include <iostream>

namespace pelican {

/**
 * @ingroup c_comms
 *
 * @class StreamDataRequest
 *
 * @brief
 * Specifications of a set of DataSpec.
 *
 * @details
 */

class StreamDataRequest : public ServerRequest
{
    private:
        QVector<DataSpec> _dataOptions;

    public:
        typedef QVector<DataSpec>::const_iterator DataSpecIterator;

    public:
        StreamDataRequest();
        ~StreamDataRequest();

        /// Adds a dataset specification that is required.
        /// The first added option will be serviced if available
        /// otherwise the server will try to satisfy subsequent options
        void addDataOption(const DataSpec& data);

        /// Returns an iterator for the DataSpec options.
        /// The order will correspond to the addDataOption call sequence.
        DataSpecIterator begin() const {return _dataOptions.begin();}

        /// Returns an iterator at the end of the DataSpec options.
        DataSpecIterator end() const {return _dataOptions.end();}

        /// Returns true if there are no stream data
        /// in any of the requirements passed.
        bool isEmpty() const { return _dataOptions.empty(); }

        /// The number of requirements.
        int size() const {return _dataOptions.size();}

        /// Test for equality between ServiceData objects.
        virtual bool operator==(const ServerRequest&) const;
};

typedef StreamDataRequest::DataSpecIterator DataSpecIterator;

} // namespace pelican
#endif // STREAMDATAREQUEST_H
