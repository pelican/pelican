#ifndef SERVICEDATAREQUEST_H
#define SERVICEDATAREQUEST_H

#include <QHash>
#include "ServerRequest.h"

/**
 * @file ServiceDataRequest.h
 */

namespace pelican {

/**
 * @class ServiceDataRequest
 *  
 * @brief
 *   A request for a specific verson of service data
 * @details
 * 
 */

class ServiceDataRequest : public ServerRequest
{

    public:
        ServiceDataRequest();
        ~ServiceDataRequest();

        // return a list of data types in the request
        QList<QString> types() const;

        // return the version of a specific type
        // n.b only one version per type allowed per request
        QString version(const QString& version) const;

    private:
        QHash<QString,QString> _dataRequested;
};

} // namespace pelican
#endif // SERVICEDATAREQUEST_H 
