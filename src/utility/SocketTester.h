#ifndef SOCKETTESTER_H
#define SOCKETTESTER_H
#include <QTcpSocket>
#include <QTcpServer>


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
        // the socket returned is the receiving socket
        QTcpSocket& send(const QByteArray&);
        QTcpSocket& receivingSocket();

    protected:
        void init();

    private:
        bool _init;
        QTcpServer _server;
        QTcpSocket _sock1;
};


} // namespace pelican
#endif // SOCKETTESTER_H 
