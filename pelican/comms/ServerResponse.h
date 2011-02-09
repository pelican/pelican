#ifndef SERVERRESPONSE_H
#define SERVERRESPONSE_H

/**
 * @file ServerResponse.h
 */

#include <QtCore/QString>

namespace pelican {

/**
 * @ingroup c_comms
 *
 * @class ServerResponse
 *
 * @brief
 * Base class to manage all responses from the pelican server.
 *
 * @details
 */
class ServerResponse
{
    public:
        typedef enum {
            Error, Acknowledge, StreamData, ServiceData, Blob, DataSupport
        } Response;

    private:
        Response _type;
        QString _msg;

    public:
        /// Constructs a new ServerResponse object.
        ServerResponse(Response type = Error, const QString& msg = "" )
        : _type(type), _msg(msg) {}

        /// Destroys the ServerResponse object.
        virtual ~ServerResponse() {}

        /// Returns the response type.
        Response type() const {return _type;}

        /// Returns the message.
        const QString& message() const {return _msg;};
};

} // namespace pelican

#endif // SERVERRESPONSE_H
