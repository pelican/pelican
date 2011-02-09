#ifndef SERVICEDATAREQUEST_H
#define SERVICEDATAREQUEST_H

/**
 * @file ServiceDataRequest.h
 */

#include <QtCore/QHash>
#include "ServerRequest.h"

namespace pelican {

/**
 * @ingroup c_comms
 *
 * @class ServiceDataRequest
 *
 * @brief
 * A request for a specific version of service data.
 *
 * @details
 */

class ServiceDataRequest : public ServerRequest
{
    public:
        ServiceDataRequest();
        ~ServiceDataRequest();

    public:
        /// Adds a request for the data object of the specified type and version.
        void request(const QString& type, const QString& version);

        /// Return a list of data types in the request.
        QList<QString> types() const;

        /// Return the version of a specific type n.b only one version per type
        /// allowed per request.
        QString version(const QString& type) const;

        /// Returns true if n valid requests() calls have been made.
        bool isEmpty() const;

        /// Test for equality between ServiceData objects.
        bool operator==(const ServiceDataRequest&) const;

        virtual bool operator==(const ServerRequest&) const;

    private:
        QHash<QString,QString> _dataRequested;
};

} // namespace pelican
#endif // SERVICEDATAREQUEST_H
