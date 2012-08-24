#include "TestDataBlobServer.h"
#include "pelican/output/PelicanTCPBlobServer.h"
#include "pelican/data/DataBlob.h"
#include "pelican/utility/ConfigNode.h"
#include <QtCore/QCoreApplication>


namespace pelican {
namespace test {


/**
 *@details TestDataBlobServer 
 */
TestDataBlobServer::TestDataBlobServer()
{
   QString xml( "<connection port=\"0\" />" );
   ConfigNode node( xml );
   _server = new PelicanTCPBlobServer( node );
}

/**
 *@details
 */
TestDataBlobServer::~TestDataBlobServer()
{
    delete _server;
}

QString TestDataBlobServer::host() const {
    return "127.0.0.1";
}

quint16 TestDataBlobServer::port() const {
    return _server->serverPort();
}

int TestDataBlobServer::clientsForStream( const QString& stream ) const {
    return _server->clientsForStream(stream);
}

void TestDataBlobServer::send(DataBlob* blob, const QString& stream ) {
    QString str=stream;
    if( str == "" ) { str=blob->type(); }
    _server->send( str, blob );
    QCoreApplication::processEvents();
}
} // namespace test
} // namespace pelican
