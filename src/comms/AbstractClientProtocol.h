#ifndef ABSTRACTCLIENTPROTOCOL_H
#define ABSTRACTCLIENTPROTOCOL_H

class QByteArray;


/**
 * @file AbstractClientProtocol.h
 */

namespace pelican {
class ServerRequest;

/**
 * @class AbstractClientProtocol
 *  
 * @brief
 *    Base class for all protocols
 * @details
 * 
 */

class AbstractClientProtocol
{
    public:
        AbstractClientProtocol(  );
        ~AbstractClientProtocol();
        virtual QByteArray serialise(const ServerRequest&) = 0;

    private:
};

} // namespace pelican
#endif // ABSTRACTCLIENTPROTOCOL_H 
