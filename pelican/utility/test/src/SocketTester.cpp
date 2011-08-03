#include "pelican/utility/test/SocketTester.h"

namespace pelican {
namespace test {

// class SocketTester
SocketTester::SocketTester()
{
    _init = false;
}

SocketTester::~SocketTester()
{
}

QTcpSocket& SocketTester::send(const QByteArray& block)
{
    init();
    if( _sock1.isOpen() ) {
        _sock1.disconnectFromHost();
    }
    Q_ASSERT( ! _server.hasPendingConnections() );
    _sock1.connectToHost(_server.serverAddress(),_server.serverPort());
    if( ! _sock1.waitForConnected(1000) ) {
        throw QString("connection timed out");
    }
    _sock1.write(block);
    _sock1.waitForBytesWritten(block.size());
    _sock1.flush();
    _server.waitForNewConnection();

    Q_ASSERT( _server.hasPendingConnections() );
    QTcpSocket* sock = _server.nextPendingConnection();
    sock->waitForReadyRead();
    Q_ASSERT(sock->bytesAvailable() == block.size());
    return *sock;
}

void SocketTester::init()
{
    if( ! _init ) {
        _server.listen(QHostAddress::LocalHost, 0);
        _init = true;
    }
}

} // namespace test
} // namespace pelican

