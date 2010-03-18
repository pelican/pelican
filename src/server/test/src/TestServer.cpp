#include <iostream>
#include "TestServer.h"
#include "PelicanPortServer.h"
#include "StreamDataBuffer.h"
#include "ServiceDataBuffer.h"
#include "WritableData.h"
#include "LockableData.h"
#include "LockableStreamData.h"
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
    if(! _proto) {
        _proto = new PelicanProtocol;
        _protoOwner = true;
    }
    start();
    // wait until we have a server object
    while( ! _server ) {}
    while( ! _server->isListening() ) {};
}

/**
 *@details
 */
TestServer::~TestServer()
{
    while (!isFinished()) quit();
    wait();
    if(_protoOwner)
        delete _proto;
    delete _server;
    wait();
}

void TestServer::run()
{
    if( ! _server  )
        _server = new PelicanPortServer(_proto,&_dm);
    if( !_server->listen(QHostAddress::Any, 0 ) )
    {
        QString msg = QString("unable to start TestServer :") + _server->errorString();
        std::cerr << msg.toStdString() << std::endl;
        return;
    }
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
    for(int i=0; i<data.size(); ++i) 
    {
        serveStreamData(data[i]);
    }
}

void TestServer::serveStreamData(const StreamData& d)
{
    StreamDataBuffer* buf = _dm.getStreamBuffer(d.name()); // ensures the StreamData buffer exists

    // The Associated Data should be registered
    // before we register the stream data
    // to ensure that the server can make the correct associations
    for(int i=0; i< d.associateData().size(); ++i )
        serveServiceData(*(d.associateData()[i].get()));

    // Now aded the Stream Data
    WritableData wd = buf->getWritable(d.size() );
    if( ! wd.isValid() ) 
        throw("unable to add StreamBuffer Data");
    // copy the object
    LockableStreamData* data = static_cast<LockableStreamData*>(wd.data());
    StreamData* datas = static_cast<StreamData*>(data->data());
    datas->setId(d.id());
    wd.write(d.operator*(), d.size());
    Q_ASSERT( datas->size() == d.size() );
}

void TestServer::serveServiceData(const QList<Data>& data )
{
    for(int i=0; i<data.size(); ++i) 
    {
        serveServiceData(data[i]);
    }
}

void TestServer::serveServiceData(const Data& d )
{
    ServiceDataBuffer* buf = _dm.getServiceBuffer(d.name()); // ensures the ServiceData buffer exists
    WritableData wd = buf->getWritable(d.size());
    if( ! wd.isValid() ) 
        throw("unable to add ServiceBuffer Data");
    wd.data()->data()->setId(d.id());
    wd.write(d.operator*(),d.size());
}

} // namespace pelican
