#include "SocketTester.h"

#include <sys/types.h>
#include <sys/socket.h>
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

QTcpSocket& SocketTester::send(QByteArray& block)
{
    init();
    _sock1.write(block);
    _sock1.flush();
    return _sock1;
}

QTcpSocket& SocketTester::receivingSocket()
{
    init();
    return _sock2;
}

void SocketTester::init()
{
    if( ! _init ) {
        int sv[2];
        Q_ASSERT( socketpair(PF_LOCAL, SOCK_STREAM, 0, sv ) );
        Q_ASSERT( _sock1.setSocketDescriptor(sv[0]) );
        Q_ASSERT( _sock2.setSocketDescriptor(sv[1]) );
    }
}

} // namespace pelican
