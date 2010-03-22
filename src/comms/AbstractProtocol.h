#ifndef ABSTRACTPROTOCOL_H
#define ABSTRACTPROTOCOL_H
#include <boost/shared_ptr.hpp>
#include <QMap>
class QString;
class QIODevice;
class QTcpSocket;

/**
 * @file AbstractProtocol.h
 */

namespace pelican {

class ServerRequest;
class StreamData;
class Data;

/**
 * @class AbstractProtocol
 *  
 * @brief
 *      Base class for all server side protocol interpretations
 * 
 * @details
 *      The protocol interprets the requests of a particular client type into 
 *      the ServerRequest Objects.
 *      Conversely it converts the internal types to be returned into the
 *      format expected by the client.
 *
 */

class AbstractProtocol
{
    public:
       typedef QList<StreamData*> StreamData_t;
       typedef QList<Data*> ServiceData_t;

    public:
        AbstractProtocol() {}
        virtual ~AbstractProtocol() {}

        /// processes an incomming request
        virtual boost::shared_ptr<ServerRequest> request(QTcpSocket& socket) = 0;

        /// write StreamData to an outgoing datastream
        virtual void send(QIODevice& stream, const StreamData_t& ) = 0;

        /// write Servicedata to an outgoing datastream
        virtual void send(QIODevice& stream, const ServiceData_t& ) = 0;

        /// write a non-error message to an outgoing datastream
        virtual void send( QIODevice& stream, const QString& message ) = 0;

        /// send an error to an outgoing datastream
        virtual void sendError(QIODevice& stream, const QString&) = 0;

    private:
};

} // namespace pelican
#endif // ABSTRACTPROTOCOL_H 
