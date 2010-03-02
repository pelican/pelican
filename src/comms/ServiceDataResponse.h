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
        const QList<QString>& types() const;
        long size(const QString& type) const;

    private:
         QList<QString> _dataTypes;
         QMap<QString,long> _sizes;
};

} // namespace pelican
#endif // SERVICEDATARESPONSE_H 
