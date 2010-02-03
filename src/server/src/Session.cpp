#include "Session.h"
#include "DataManager.h"
#include "LockedData.h"
#include "StreamData.h"
#include "comms/AbstractProtocol.h"
#include "comms/ServerRequest.h"
#include "comms/StreamDataRequest.h"
#include "comms/ServiceDataRequest.h"
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
    QTcpSocket socket;
    if (!socket.setSocketDescriptor(_socketDescriptor)) {
        emit error(socket.error());
        return;
    }

    ServerRequest req = _proto->request(socket);

    QByteArray block;
    processRequest(req, block);
    socket.write(block); 
    socket.disconnectFromHost();
    socket.waitForDisconnected();
}

/*
 * @details
 * this routine processes a general ServerRequest, calling the appropriate
 * working and then passes this on to the protocol to be returned to the client
 */
void Session::processRequest(const ServerRequest& req, QByteArray& out)
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
 * returns all the datasets mentioned in the list (or throws if any one is missing)
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
            throw QString("Data Requested does not exist :" + type + " " + req.version(type));
        }
        data.append(d);
    }
    return data;
}

} // namespace pelican
