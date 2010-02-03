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
 *    Base class for all protocols on the Server Side
 * @details
 * 
 */

class AbstractClientProtocol
{
    public:
        AbstractClientProtocol(  );
        ~AbstractClientProtocol();
        virtual QByteArray serialise(const ServerRequest&) = 0;
        virtual boost::shared_ptr<ServerResponse> receive(QAbstractSocket&) = 0;

    private:
};

} // namespace pelican
#endif // ABSTRACTCLIENTPROTOCOL_H 
