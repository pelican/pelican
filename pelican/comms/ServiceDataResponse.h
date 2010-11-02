#ifndef SERVICEDATARESPONSE_H
#define SERVICEDATARESPONSE_H

/**
 * @file ServiceDataResponse.h
 */

#include "ServerResponse.h"
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>

namespace pelican {

class DataChunk;

/**
 * @ingroup c_comms
 *
 * @class ServiceDataResponse
 *
 * @brief
 * Contains the response from the server containing a Service Data Set.
 *
 * @details
 */

class ServiceDataResponse : public ServerResponse
{
    public:
        /// Creates the ServiceDataResponse object.
        ServiceDataResponse();

        /// Destroys the ServiceDataResponse object.
        ~ServiceDataResponse();

    public:
        /// Add a Data object to the response list.
        void addData(DataChunk*);

        /// Return a list of valid data objects.
        const QList<DataChunk*>& data() const {return _data;}

    private:
         QList<DataChunk*> _data;
};

} // namespace pelican
#endif // SERVICEDATARESPONSE_H
