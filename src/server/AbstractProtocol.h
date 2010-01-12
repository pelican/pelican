#ifndef ABSTRACTPROTOCOL_H
#define ABSTRACTPROTOCOL_H
#include <QMap>
class QString;
class QDataStream;
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
 *      Base class for all server protocols interpretations
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
        typedef QMap<QString,StreamData*> StreamData_t;
        typedef QMap<QString,Data*> ServiceData_t;

    public:
        AbstractProtocol();
        virtual ~AbstractProtocol();
        virtual ServerRequest request(QTcpSocket& socket) = 0;
        virtual void send(QDataStream& stream, const StreamData_t& ) = 0;
        virtual void send(QDataStream& stream, const ServiceData_t& ) = 0;
        virtual void send( QDataStream& stream, const QString& message ) = 0;
        virtual void sendError(QDataStream& stream, const QString&) = 0;

    private:
};

} // namespace pelican
#endif // ABSTRACTPROTOCOL_H 
