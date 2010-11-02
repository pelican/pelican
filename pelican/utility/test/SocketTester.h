#ifndef SOCKETTESTER_H
#define SOCKETTESTER_H

/**
 * @file SocketTester.h
 */

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

namespace pelican {
namespace test {

/**
 * @ingroup t_utility
 *
 * @class SocketTester
 *
 * @brief
 * Interface to allow unit testing of socket communications
 *
 * @details
 * Class to assist in testing methods that take a QTcpSocket as an argument.
 * Call the send method with the required data stream and a QTcpSocket that
 * receives the data will be provided for passing to the method.
 */

class SocketTester
{
    public:
        SocketTester();
        ~SocketTester();

    public:
        /// Send some data to the socket the socket returned is the receiving
        /// socket.
        QTcpSocket& send(const QByteArray&);

    protected:
        void init();

    private:
        bool _init;
        QTcpServer _server;
        QTcpSocket _sock1;
};

} // namespace test
} // namespace pelican
#endif // SOCKETTESTER_H
