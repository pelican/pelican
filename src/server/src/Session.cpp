#include "Session.h"
#include "AbstractProtocol.h"
#include "DataManager.h"
#include "LockedData.h"
#include "StreamData.h"
#include "data/ServerRequest.h"
#include "data/StreamDataRequest.h"
#include "data/ServiceDataRequest.h"
#include <QTcpSocket>
#include <QString>
#include <QHash>
#include <iostream>
#include "utility/memCheck.h"

namespace pelican {

// class Session 
Session::Session(int socketDescriptor, AbstractProtocol* proto, DataManager* data, QObject* parent)
    : QThread(parent), _data(data)
{
    _proto = proto;
    _socketDescriptor = socketDescriptor;
}

Session::~Session()
{
}

void Session::run()
{
    // setup the output stream
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0; // this should really be in the protocol

    QTcpSocket socket;
    if (!socket.setSocketDescriptor(_socketDescriptor)) {
        return;
    }
    //ServerRequest req = _proto->request(socket);
    //processRequest(req, out);
    out << "Hello";

    out.device()->seek(0);   // TODO move to protocol
    out << (quint16)(block.size() - sizeof(quint16)); // TODO move to protocol

    socket.write(block); 
    socket.disconnectFromHost();
    socket.waitForDisconnected();
}

/*
 * @details
 * this routine processes a general ServerRequest, calling the appropriate
 * working and then passes this on to the protocol to be returned to the client
 */
void Session::processRequest(const ServerRequest& req, QDataStream& out)
{
    try {
        switch(req.type()) {
            case ServerRequest::DataSupport:
                _proto->send(out,"ACK");
                break;
            case ServerRequest::StreamData:
                {
                    QList<LockedData> d = processStreamDataRequest(static_cast<const StreamDataRequest&>(req) );
                    if( d.size() ) {
                        AbstractProtocol::StreamData_t data;
                        foreach( LockedData ld, d) {
                            data.insert(ld.name(), static_cast<StreamData*>(ld.data()));
                        }
                        _proto->send( out, data );
                    }
                }
                break;
            case ServerRequest::ServiceData:
                {
                    QList<LockedData> d = processServiceDataRequest(static_cast<const ServiceDataRequest&>(req) );
                    if( d.size() ) {
                        AbstractProtocol::ServiceData_t data;
                        foreach( LockedData ld, d) {
                            data.insert(ld.name(), ld.data());
                        }
                        _proto->send( out, data );
                    }
                }
                break;
            default:
                _proto->sendError( out, req.message());
        }
    }
    catch( const QString& e )
    {
        _proto->sendError( out, e );
    }
}

/*
 * @details
 * iterates over the list of data options provided in the request
 * It will take the first dataset that matches and that has data.
 * The data will be returned as a locked container to ensure access
 * by other threads will be blocked.
 */
QList<LockedData> Session::processStreamDataRequest(const StreamDataRequest& req )
{
    QList<LockedData> data;
    DataRequirementsIterator it=req.begin();
    while( it != req.end() && data.size() == 0 )
    {
        if( ! it->isCompatible( _data->dataSpec() ) )
            throw QString("data requested not supported by server");
        // attempt to get the required stream data
        foreach (const QString stream, it->streamData() )
        {
            //std::cout << "stream: " << stream.toStdString() << std::endl;
            LockedData d = _data->getNext(stream, it->serviceData() );
            if( ! d.isValid() ) {
                data.clear();
                break; // one invalid stream invalidates the request
            }
            data.append(d);
        }
        ++it;
    }
    return data;
}

/*
 * @details
 * aquires all the datasets mentioned in the list (or none if any one is missing)
 * The data will be returned as a locked containeris to ensure access
 * by other threads will be blocked.
 */
QList<LockedData> Session::processServiceDataRequest(const ServiceDataRequest& req )
{
    QList<LockedData> data;
    foreach( QString type, req.types() ) {
        LockedData d = _data->getServiceData(type, req.version(type));
        if( ! d.isValid() )
        {
            data.clear();
            break;
        }
        data.append(d);
    }
    return data;
}

} // namespace pelican
