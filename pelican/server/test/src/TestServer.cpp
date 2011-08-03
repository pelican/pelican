#include "pelican/server/test/TestServer.h"

#include "pelican/server/PelicanPortServer.h"
#include "pelican/server/StreamDataBuffer.h"
#include "pelican/server/ServiceDataBuffer.h"
#include "pelican/server/WritableData.h"
#include "pelican/server/LockableServiceData.h"
#include "pelican/server/LockableStreamData.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/comms/StreamData.h"
#include "pelican/comms/DataChunk.h"
#include "pelican/utility/Config.h"

#include <iostream>

namespace pelican {
namespace test {


/**
 * @details
 * The default protocol is the standard PelicanProtocol, which can be
 * overridden in the constructor.
 */
TestServer::TestServer(AbstractProtocol* proto,  QObject* parent)
    : QThread(parent), _protoOwner(false),
      _proto(proto), _portServer(0)
{
    // set up the protocol
    if(! _proto) {
        _proto = new PelicanProtocol;
        _protoOwner = true;
    }
    start();
    // wait until we have a server object
    while( ! _portServer ) {msleep(1);}
    while( ! _portServer->isListening() ) {msleep(1);};
}

/**
 *@details
 */
TestServer::~TestServer()
{
    while (!isFinished()) quit();
    if(_protoOwner)
        delete _proto;
    if (_dataManager)
        delete _dataManager;
    delete _portServer;
    wait();
}

void TestServer::run()
{
    Config config;
    _dataManager = new DataManager(&config);;
    if( ! _portServer  ) {
        _portServer = new PelicanPortServer(_proto, _dataManager);
    }
    if( !_portServer->listen(QHostAddress::Any, 0 ) )
    {
        QString msg = QString("unable to start TestServer :");
        msg += _portServer->errorString();
        std::cerr << msg.toStdString() << std::endl;
        return;
    }
    exec();
}


quint16 TestServer::port() const
{
    Q_ASSERT( isRunning() );
    return _portServer->serverPort();
}

bool TestServer::isListening() const
{
    Q_ASSERT( isRunning() );
    return _portServer->isListening();
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
    // ensures the StreamData buffer exists
    StreamDataBuffer* buf = _dataManager->getStreamBuffer(d.name());

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
    StreamData* datas = data->streamData();
    datas->setId(d.id());
    wd.write(d.ptr(), d.size());
    Q_ASSERT( datas->size() == d.size() );
}

void TestServer::serveServiceData(const QList<DataChunk>& data )
{
    for(int i=0; i<data.size(); ++i)
    {
        serveServiceData(data[i]);
    }
}

void TestServer::serveServiceData(const DataChunk& d )
{
    // ensures the ServiceData buffer exists
    ServiceDataBuffer* buf = _dataManager->getServiceBuffer(d.name());
    WritableData wd = buf->getWritable(d.size());
    if( ! wd.isValid() )
        throw("unable to add ServiceBuffer Data");
    DataChunk* data = wd.data()->data().get();
    data->setId(d.id());
    wd.write(d.ptr(), d.size());
}

} // namespace test
} // namespace pelican
