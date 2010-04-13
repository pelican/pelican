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
 * Base class for all server side protocol interpretations.
 * 
 * @details
 * The protocol interprets the requests of a particular client type into
 * the ServerRequest Objects.
 * Conversely it converts the internal types to be returned into the
 * format expected by the client.
 */
class AbstractProtocol
{
    public:
       typedef QList<StreamData*> StreamData_t;
       typedef QList<Data*> ServiceData_t;

    public:
        AbstractProtocol() {}
        virtual ~AbstractProtocol() {}

        /// Processes an incoming request.
        virtual boost::shared_ptr<ServerRequest> request(QTcpSocket& socket) = 0;

        /// Write stream data to an I/O device.
        virtual void send(QIODevice& device, const StreamData_t&) = 0;

        /// Write service data to an I/O device.
        virtual void send(QIODevice& device, const ServiceData_t&) = 0;

        /// Write a non-error message to an I/O device.
        virtual void send(QIODevice& device, const QString& message) = 0;

        /// Send an error to an I/O device.
        virtual void sendError(QIODevice& device, const QString&) = 0;
};

} // namespace pelican

#endif // ABSTRACTPROTOCOL_H 
