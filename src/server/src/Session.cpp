#include "Session.h"
#include "AbstractProtocol.h"
#include "DataManager.h"
#include "data/ServerRequest.h"
#include <QTcpSocket>
#include <QString>
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

Session::~Session()
{
}

} // namespace pelican
