#include <iostream>
#include "TestServer.h"
#include "PelicanPortServer.h"
#include "StreamDataBuffer.h"
#include "ServiceDataBuffer.h"
#include "WritableData.h"
#include "LockableData.h"
#include "comms/PelicanProtocol.h"
#include "comms/StreamData.h"
#include "comms/Data.h"


#include "utility/memCheck.h"

namespace pelican {


/**
 *@details TestServer 
   The default portocol is the standard PelicanProtocol, which can be
   overridden in the constructor.
 */
TestServer::TestServer(AbstractProtocol* proto,  QObject* parent)
    : QThread(parent), _protoOwner(false), 
      _proto(proto), _server(0)
{
    // set up the protocol
    if(_proto) {
        _proto = new PelicanProtocol;
        _protoOwner = true;
    }
    start();
    // wait until we have a server object
    while( ! _server ) {}
}

/**
 *@details
 */
TestServer::~TestServer()
{
    quit();
    if(_protoOwner)
        delete _proto;
    delete _server;
}

void TestServer::run()
{
    if( ! _server  )
        _server = new PelicanPortServer(_proto,&_dm);
    if( !_server->listen(QHostAddress::Any, 0 ) )
    {
        QString msg = QString("unable to start TestServer :") + _server->errorString();
        std::cerr << msg.toStdString() << std::endl;
        quit();
    }
    Q_ASSERT(isListening());
    exec();
}


quint16 TestServer::port() const
{
    Q_ASSERT( isRunning() );
    return _server->serverPort();
}

bool TestServer::isListening() const
{
    Q_ASSERT( isRunning() );
    return _server->isListening();
}

void TestServer::serveStreamData(const QList<StreamData>& data)
{
    foreach( StreamData d, data )
    {
        serveStreamData(d);
    }
}

void TestServer::serveStreamData(const StreamData& d)
{
    StreamDataBuffer* buf = _dm.getStreamBuffer(d.name()); // ensures the StreamData buffer exists
    WritableData wd = buf->getWritable(d.size());
    *(wd.data()->data()) = d;
}

void TestServer::serveServiceData(const QList<Data>& data )
{
    foreach( Data d, data )
    {
        serveServiceData(d);
    }
}

void TestServer::serveServiceData(const Data& d )
{
    ServiceDataBuffer* buf = _dm.getServiceBuffer(d.name()); // ensures the ServiceData buffer exists
    WritableData wd = buf->getWritable(d.size());
    *(wd.data()->data()) = d;
}

} // namespace pelican
