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

#ifndef TCPCONNECTIONMANAGERTEST_H
#define TCPCONNECTIONMANAGERTEST_H

/**
 * @file TCPConnectionManagerTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
class QTcpSocket;

namespace pelican {
class ServerRequest;
class TCPConnectionManager;
class PelicanClientProtocol;

/**
 * @ingroup t_output
 *
 * @class TCPConnectionManagerTest
 *
 * @brief
 * Unit test for the TCPConnectionManager
 *
 * @details
 */

class TCPConnectionManagerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( TCPConnectionManagerTest );
        CPPUNIT_TEST( test_send );
        CPPUNIT_TEST( test_brokenConnection );
        CPPUNIT_TEST( test_dataSupportedRequest );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_send();
        void test_brokenConnection();
        void test_dataSupportedRequest();

    public:
        TCPConnectionManagerTest();
        ~TCPConnectionManagerTest();

    private:
        QTcpSocket* _createClient() const;
        void _sendRequest(QTcpSocket* tcpSocket, const ServerRequest& req);
        TCPConnectionManager* _server;
        PelicanClientProtocol* _clientProtocol;
};

} // namespace pelican
#endif // TCPCONNECTIONMANAGERTEST_H
