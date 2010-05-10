#ifndef ABSTRACTCLIENTPROTOCOL_H
#define ABSTRACTCLIENTPROTOCOL_H

#include <boost/shared_ptr.hpp>
#include "pelican/utility/Factory.h"
class QByteArray;
class QAbstractSocket;

/**
 * @file AbstractClientProtocol.h
 */

namespace pelican {

class ServerRequest;
class ServerResponse;
class DataBlob;

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
        AbstractClientProtocol();

        /// Destroys the AbstractClientProtocol (virtual).
        virtual ~AbstractClientProtocol();

        /// Serialises the ServerRequest (pure virtual).
        virtual QByteArray serialise(const ServerRequest&) = 0;

        /// Translate incomming bit stream from a socket into
        //  appropriate ServerResponse objects
        virtual boost::shared_ptr<ServerResponse> receive(QAbstractSocket&) = 0;

    protected:
        /// find a datablob object
        DataBlob* _getDataBlob(const QString& type);

    private:
        Factory<DataBlob> _blobFactory;

};

} // namespace pelican

#endif // ABSTRACTCLIENTPROTOCOL_H 
