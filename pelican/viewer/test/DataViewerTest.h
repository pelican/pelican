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

#ifndef DATAVIEWERTEST_H
#define DATAVIEWERTEST_H

/**
 * @file DataViewerTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
#include <QtCore/QString>

class QApplication;

namespace pelican {

class DataBlobClient;
class PelicanTCPBlobServer;

/**
 * @ingroup t_viewer
 *
 * @class DataViewerTest
 *
 * @brief
 * Unit test for the DataViewer
 *
 * @details
 */

class DataViewerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataViewerTest );
        CPPUNIT_TEST( test_client );
        CPPUNIT_TEST( test_integrationWithDataClient);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_client();
        void test_integrationWithDataClient();

    public:
        /// DataViewerTest constructor.
        DataViewerTest();

        /// DataViewerTest destructor.
        virtual ~DataViewerTest();

    private:
        DataBlobClient* _client( PelicanTCPBlobServer* server );
};

CPPUNIT_TEST_SUITE_REGISTRATION( DataViewerTest );

} // namespace pelican
#endif // DATAVIEWERTEST_H
