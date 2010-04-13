#ifndef ABSTRACTCLIENTPROTOCOL_H
#define ABSTRACTCLIENTPROTOCOL_H

#include <boost/shared_ptr.hpp>
class QByteArray;
class QAbstractSocket;

/**
 * @file AbstractClientProtocol.h
 */

namespace pelican {

class ServerRequest;
class ServerResponse;

/**
 * @class AbstractClientProtocol
 *  
 * @brief
 * Base class for all protocols on the client side.
 *
 * @details
 * 
 */
class AbstractClientProtocol
{
    public:
        /// Creates a new AbstractClientProtocol.
        AbstractClientProtocol() {};

        /// Destroys the AbstractClientProtocol (virtual).
        virtual ~AbstractClientProtocol() {};

        /// Serialises the ServerRequest (pure virtual).
        virtual QByteArray serialise(const ServerRequest&) = 0;

        /// TODO Some description here?
        virtual boost::shared_ptr<ServerResponse> receive(QAbstractSocket&) = 0;
};

} // namespace pelican

#endif // ABSTRACTCLIENTPROTOCOL_H 
