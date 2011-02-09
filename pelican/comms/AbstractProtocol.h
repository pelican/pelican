#ifndef ABSTRACTPROTOCOL_H
#define ABSTRACTPROTOCOL_H

/**
 * @file AbstractProtocol.h
 */

#include <boost/shared_ptr.hpp>
#include <QtCore/QMap>

class QString;
class QIODevice;
class QTcpSocket;

namespace pelican {

class ServerRequest;
class StreamData;
class DataChunk;
class DataBlob;
class DataSupportResponse;

/**
 * @ingroup c_comms
 *
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
       typedef QList<DataChunk*> ServiceData_t;

    public:
        AbstractProtocol() {}
        virtual ~AbstractProtocol() {}

        /// Processes an incoming request.
        virtual boost::shared_ptr<ServerRequest> request(QTcpSocket& socket) = 0;

        /// Write stream data to an I/O device.
        virtual void send(QIODevice& device, const StreamData_t&) = 0;

        /// Write service data to an I/O device.
        virtual void send(QIODevice& device, const ServiceData_t&) = 0;

        /// Write out a DataBlob object to an I/O Device, as a stream of "name"
        virtual void send(QIODevice& device, const QString& name, const DataBlob& ) = 0;

        /// Write a non-error message to an I/O device.
        virtual void send(QIODevice& device, const QString& message) = 0;

        /// Write a non-error message to an I/O device.
        virtual void send(QIODevice& device, const DataSupportResponse& message) = 0;

        /// Send an error to an I/O device.
        virtual void sendError(QIODevice& device, const QString&) = 0;

};

} // namespace pelican

#endif // ABSTRACTPROTOCOL_H
