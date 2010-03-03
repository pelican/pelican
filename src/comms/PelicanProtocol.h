#ifndef PELICANPROTOCOL_H
#define PELICANPROTOCOL_H


#include "AbstractProtocol.h"

/**
 * @file PelicanProtocol.h
 */

namespace pelican {

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
        virtual void send(QIODevice& stream, const AbstractProtocol::StreamData_t& );
        virtual void send(QIODevice& stream, const AbstractProtocol::ServiceData_t& );
        virtual void send(QIODevice& stream, const QString& );
        virtual void sendError(QIODevice& stream, const QString&);

    private:
};

} // namespace pelican
#endif // PELICANPROTOCOL_H 
