#ifndef SERVERREQUEST_H
#define SERVERREQUEST_H

/**
 * @file ServerRequest.h
 */

#include <QtCore/QString>

namespace pelican {

/**
 * @ingroup c_comms
 *
 * @class ServerRequest
 *
 * @brief
 * Encapsulates the data requests to the server.
 *
 * @details
 * Requests must have a type and ancillary data.
 */

class ServerRequest
{
    public:
        typedef enum {
            Error, Acknowledge, StreamData, ServiceData, DataSupport
        } Request;

    private:
        Request _type;
        QString _error;

    public:
        /// Constructs a new ServerRequest object.
        ServerRequest(Request type = Error, const QString& msg = "")
        : _type(type), _error(msg) {}

        /// Destroys the ServerRequest object.
        virtual ~ServerRequest() {}

        /// Returns the request type.
        Request type() const {return _type;}

        /// Sets the error message.
        void error(const QString& msg) {_error = msg; _type = Error;}

        /// Returns the error message.
        const QString& message() const {return _error;}

        /// Tests whether this ServerRequest type is the same as another.
        virtual bool operator==(const ServerRequest& req) const
        { return _type == req._type; }
};

} // namespace pelican

#endif // SERVERREQUEST_H
