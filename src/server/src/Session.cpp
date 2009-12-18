#include "Session.h"
#include "AbstractProtocol.h"
#include "DataManager.h"
#include "LockedData.h"
#include "data/ServerRequest.h"
#include "data/StreamDataRequest.h"
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

void Session::processRequest(const ServerRequest& req, QDataStream& out)
{
    switch(req.type()) {
        case ServerRequest::DataSupport:
            _proto->send(out,"ACK");
            break;
        case ServerRequest::StreamData:
        //QList<DataBlob> datablobs;
        //_proto->send( out, datablobs  );
            break;
        case ServerRequest::ServiceData:
            break;
        default:
            _proto->sendError( out, req.message());
    }
}

void Session::processStreamDataRequest(const StreamDataRequest& req )
{
    QList<QString> streams = _data->streams();
    QList<QString> serviceData = _data->serviceData();
    DataRequirementsIterator it=req.begin();
    while( it != req.end() )
    {
        // attempt to get the required stream data
        //QSet<QString> streams =req->streams();
        //{
         //   LockedData d = _data->getNext(stream);
         //   if( ! d.isValid() ) {
         //       break;
         //   }
       // }
    }
}

} // namespace pelican
