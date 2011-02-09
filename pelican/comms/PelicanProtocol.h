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

    public:
        /// Construct a server request object from reading the specified socket
        virtual boost::shared_ptr<ServerRequest> request(QTcpSocket& socket);

        /// Sends a list of supported stream and service data.
        virtual void send(QIODevice& device, const DataSupportResponse&);

        /// Send one or more stream data chunks with header information
        /// containing a description of associated service data.
        virtual void send(QIODevice& stream, const AbstractProtocol::StreamData_t&);

        /// Send a serialised data blob.
        virtual void send(QIODevice& stream, const QString& name, const DataBlob&);

        /// Send one or more service data chunks.
        virtual void send(QIODevice& stream, const AbstractProtocol::ServiceData_t&);

        /// Send a message string.
        virtual void send(QIODevice& stream, const QString&);

        /// Send a error.
        virtual void sendError(QIODevice& stream, const QString&);
};

} // namespace pelican
#endif // PELICANPROTOCOL_H
