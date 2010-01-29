#ifndef SOCKETTESTER_H
#define SOCKETTESTER_H
#include <QTcpSocket>


/**
 * @file SocketTester.h
 */

namespace pelican {

/**
 * @class SocketTester
 *  
 * @brief
 *    C++ interface to allow unit testing of socket communications
 * @details
 * 
 */

class SocketTester
{
    public:
        SocketTester();
        ~SocketTester();
        // send some data to the socket
        QTcpSocket& send(const QByteArray&);
        QTcpSocket& receivingSocket();

    protected:
        void init();

    private:
        bool _init;
        QTcpSocket _sock1;
        QTcpSocket _sock2;
};


} // namespace pelican
#endif // SOCKETTESTER_H 
