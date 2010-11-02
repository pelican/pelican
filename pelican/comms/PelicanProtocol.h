#ifndef PELICANPROTOCOL_H
#define PELICANPROTOCOL_H

/**
 * @file PelicanProtocol.h
 */

#include "AbstractProtocol.h"


namespace pelican {

class DataBlob;

/**
 * @ingroup c_comms
 *
 * @class PelicanProtocol
 *
 * @brief
 * The primary protocol for communication between pipelines and the server.
 *
 * @details
 */

class PelicanProtocol : public AbstractProtocol
{
    public:
        PelicanProtocol();
        ~PelicanProtocol();
        virtual boost::shared_ptr<ServerRequest> request(QTcpSocket& socket);
        virtual void send(QIODevice& stream, const AbstractProtocol::StreamData_t& );
        virtual void send(QIODevice& stream, const AbstractProtocol::ServiceData_t& );
        virtual void send(QIODevice& device, const DataSupportResponse&);
        virtual void send(QIODevice& stream, const QString& );
        virtual void send(QIODevice& stream, const QString& name, const DataBlob& );
        virtual void sendError(QIODevice& stream, const QString&);
};

} // namespace pelican
#endif // PELICANPROTOCOL_H
