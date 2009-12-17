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
        enum private_Request_t { Error, Acknowledge, StreamData, ServiceData, DataSupport };
        typedef private_Request_t Request_t;

    public:

        ServerRequest(Request_t type = Error, const QString& msg = "");
        ~ServerRequest();
        Request_t type() const;

        QString message() const;

    private:
        Request_t _type;
        QString _error;
};

} // namespace pelican
#endif // SERVERREQUEST_H 
