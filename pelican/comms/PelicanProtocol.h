#ifndef PELICANPROTOCOL_H
#define PELICANPROTOCOL_H


#include "AbstractProtocol.h"

/**
 * @file PelicanProtocol.h
 */

namespace pelican {

    class DataBlob;

/**
 * @class PelicanProtocol
 *
 * @brief
 *   The primary protocol for communication between pipelines and
 *   the server
 * @details
 *
 */

class PelicanProtocol : public AbstractProtocol
{
    public:
        PelicanProtocol(  );
        ~PelicanProtocol();
        virtual boost::shared_ptr<ServerRequest> request(QTcpSocket& socket);
        virtual void send(QIODevice& stream, const AbstractProtocol::StreamDataList& );
        virtual void send(QIODevice& stream, const AbstractProtocol::ServiceDataList& );
        virtual void send(QIODevice& device, const DataSupportResponse&);
        virtual void send(QIODevice& stream, const QString& );
        virtual void send(QIODevice& stream, const QString& name, const DataBlob& );
        virtual void sendError(QIODevice& stream, const QString&);

    private:
        unsigned long _counter;
};

} // namespace pelican
#endif // PELICANPROTOCOL_H
