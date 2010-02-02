#include "SocketTester.h"

//#include <sys/types.h>
//#include <sys/socket.h>
#include "utility/memCheck.h"

namespace pelican {

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
    _sock1.connectToHost(_server.serverAddress(),_server.serverPort());
    if( ! _sock1.waitForConnected(1000) ) {
        throw QString("connection timed out");
    }
    _sock1.write(block);
    _sock1.flush();
    _server.waitForNewConnection();
    return *(_server.nextPendingConnection());
}

void SocketTester::init()
{
    if( ! _init ) {
        _server.listen(QHostAddress::LocalHost,0);
    }
}

} // namespace pelican

