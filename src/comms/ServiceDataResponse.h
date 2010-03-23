#ifndef SERVICEDATARESPONSE_H
#define SERVICEDATARESPONSE_H

#include <QList>
#include <QMap>
#include <QString>
#include "ServerResponse.h"

/**
 * @file ServiceDataResponse.h
 */

namespace pelican {

class Data;

/**
 * @class ServiceDataResponse
 *  
 * @brief
 * Contains the response from the server containing a Service Data Set.
 *
 * @details
 * 
 */
class ServiceDataResponse : public ServerResponse
{
    private:
         QList<Data*> _data;

    public:
        /// Creates the ServiceDataResponse object.
        ServiceDataResponse();

        /// Destroys the ServiceDataResponse object.
        ~ServiceDataResponse();

        /// Add a Data object to the response list.
        void addData(Data*);

        /// Return a list of valid data objects.
        const QList<Data*>& data() const {return _data;}
};

} // namespace pelican

#endif // SERVICEDATARESPONSE_H 
