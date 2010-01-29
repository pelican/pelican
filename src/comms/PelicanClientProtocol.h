#ifndef PELICANCLIENTPROTOCOL_H
#define PELICANCLIENTPROTOCOL_H


#include "AbstractClientProtocol.h"

/**
 * @file PelicanClientProtocol.h
 */

namespace pelican {

/**
 * @class PelicanClientProtocol
 *  
 * @brief
 *   Implementation of the pelican protocol client side
 * @details
 * 
 */

class PelicanClientProtocol : public AbstractClientProtocol
{
    public:
        PelicanClientProtocol(  );
        ~PelicanClientProtocol();
        virtual QByteArray serialise(const ServerRequest&);

    private:
};

} // namespace pelican
#endif // PELICANCLIENTPROTOCOL_H 
