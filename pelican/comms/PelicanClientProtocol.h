#ifndef PELICANCLIENTPROTOCOL_H
#define PELICANCLIENTPROTOCOL_H

/**
 * @file PelicanClientProtocol.h
 */

#include "AbstractClientProtocol.h"

class QDataStream;

namespace pelican {

class DataRequirements;

/**
 * @ingroup c_comms
 *
 * @class PelicanClientProtocol
 *
 * @brief
 * Implementation of the PelicanProtocol for the client side of communications.
 *
 * @details
 */

class PelicanClientProtocol : public AbstractClientProtocol
{
    public:
        PelicanClientProtocol();
        ~PelicanClientProtocol();
        virtual QByteArray serialise(const ServerRequest&);
        virtual boost::shared_ptr<ServerResponse> receive(QAbstractSocket&);

    private:
        void _serializeDataRequirements(QDataStream& stream,
                const DataRequirements& req) const;
};

} // namespace pelican
#endif // PELICANCLIENTPROTOCOL_H
