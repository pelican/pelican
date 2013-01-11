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

#ifndef PIPELINEDRIVERTEST_H
#define PIPELINEDRIVERTEST_H

/**
 * @file PipelineDriverTest.h
 */

#include "data/DataBlob.h"
#include "modules/AbstractModule.h"
#include "utility/FactoryConfig.h"
#include "utility/FactoryGeneric.h"

#include <cppunit/extensions/HelperMacros.h>

class QCoreApplication;

namespace pelican {

class PipelineDriver;
class DataClientFactory;
class OutputStreamManager;
class AbstractDataClient;

/**
 * @ingroup t_core
 *
 * @class PipelineDriverTest
 *
 * @brief
 * CppUnit tests for PipelineDriver class.
 *
 * @details
 *
 */
class PipelineDriverTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( PipelineDriverTest );
        CPPUNIT_TEST( test_checkPipelineRequirements);
        CPPUNIT_TEST( test_registerPipeline );
        CPPUNIT_TEST( test_registerSwitcher );
/*
        CPPUNIT_TEST( test_registerSwitcherData );
        CPPUNIT_TEST( test_registerPipeline_null );
        CPPUNIT_TEST( test_start_noPipelinesRegistered );
        CPPUNIT_TEST( test_start_noPipelinesRun );
        CPPUNIT_TEST( test_start_singlePipelineNoClient );
        CPPUNIT_TEST( test_start_singlePipelineClientReturnsGoodData );
        CPPUNIT_TEST( test_start_singlePipelineClientReturnsWrongData );
        CPPUNIT_TEST( test_start_multiPipelineRunDifferentData );
        CPPUNIT_TEST( test_start_multiPipelineRunOne );
        CPPUNIT_TEST( test_start_pipelineWithHistory );
*/
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_checkPipelineRequirements();
        void test_registerPipeline();
        void test_registerSwitcher();
        void test_registerSwitcherData();
        void test_registerPipeline_null();
        void test_start_noPipelinesRegistered();
        void test_start_noPipelinesRun();
        void test_start_singlePipelineNoClient();
        void test_start_singlePipelineClientReturnsGoodData();
        void test_start_singlePipelineClientReturnsWrongData();
        void test_start_multiPipelineRunDifferentData();
        void test_start_multiPipelineRunOne();
        void test_start_pipelineWithHistory();

    public:
        PipelineDriverTest(  );
        ~PipelineDriverTest();

    private:
        QCoreApplication *_coreApp;
        PipelineDriver *_pipelineDriver;
        FactoryGeneric<DataBlob>* _dataBlobFactory;
        FactoryConfig<AbstractModule>* _moduleFactory;
        DataClientFactory* _clientFactory;
        OutputStreamManager* _osmanager;
        AbstractDataClient* _client;
        void  _setTestClient();
};

} // namespace pelican

#endif // PIPELINEDRIVERTEST_H
