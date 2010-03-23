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
        typedef enum { Error, Acknowledge, StreamData, ServiceData, DataSupport } Request;

    public:

        ServerRequest(Request type = Error, const QString& msg = "");
        virtual ~ServerRequest();
        Request type() const;

        void error(const QString& msg);
        QString message() const;
        virtual bool operator==(const ServerRequest&) const;

    private:
        Request _type;
        QString _error;
};

} // namespace pelican
#endif // SERVERREQUEST_H
