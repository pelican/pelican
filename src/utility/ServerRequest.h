#ifndef SERVERREQUEST_H
#define SERVERREQUEST_H


#include <QStringList>
#include <QString>

/**
 * @file ServerRequest.h
 */

namespace pelican {

/**
 * @class ServerRequest
 *  
 * @brief
 *   Encapsulates the data requests to the server.
 * @details
 *   Requests must have a type and ancillary data
 */

class ServerRequest
{
    public:
        ServerRequest(const QStringList& tokens);
        ServerRequest(const QString& type);
        ~ServerRequest();
        bool error() const;
        const QString& type() const;
        void setType(const QString&);

        QString errorMessage() const;


    private:
        QString _error;
        QString _type;
};

} // namespace pelican
#endif // SERVERREQUEST_H 
