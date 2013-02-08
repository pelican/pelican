/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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
