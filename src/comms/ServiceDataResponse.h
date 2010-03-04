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
 *    Contains the response from the server containing a Service Data Set
 * @details
 * 
 */

class ServiceDataResponse : public ServerResponse
{
    public:
        ServiceDataResponse(  );
        ~ServiceDataResponse();
        /// add a Data object to the response list
        //  this class will take ownership of the Data 
        //  object and delete it on destruction
        void addData(Data*);

        /// return a list of valid data objects
        const QList<Data*>& data() const;

    private:
         QList<Data*> _data;
};

} // namespace pelican
#endif // SERVICEDATARESPONSE_H 
