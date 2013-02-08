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

#ifndef PIPELINEAPPLICATIONTEST_H
#define PIPELINEAPPLICATIONTEST_H

/**
 * @file PipelineApplicationTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_core
 *
 * @class PipelineApplicationTest
 *
 * @brief
 * CppUnit tests for PipelineApplication class.
 *
 * @details
 * Tries to create a PipelineApplication:
 * - before a QCoreApplication
 * - after a QCoreApplication
 *
 * Tests parsing of command line arguments:
 * - for setting configuration file
 * - for printing a help message
 *
 * Tests registering one pipeline.
 * Tests registering two pipelines.
 * Tests starting the pipeline driver.
 */
class PipelineApplicationTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE(PipelineApplicationTest);
//        CPPUNIT_TEST(test_create_beforeQCoreApplication);
//        CPPUNIT_TEST(test_create_afterQCoreApplication);
//        CPPUNIT_TEST(test_commandLine_config);
//        CPPUNIT_TEST(test_commandLine_help);
//        CPPUNIT_TEST(test_registerPipeline_EmptyPipeline);
//        CPPUNIT_TEST(test_start);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_create_beforeQCoreApplication();
        void test_create_afterQCoreApplication();
        void test_commandLine_config();
        void test_commandLine_help();
        void test_registerPipeline_EmptyPipeline();
        void test_start();

    public:
        PipelineApplicationTest(  );
        ~PipelineApplicationTest();

    private:
};

} // namespace pelican

#endif // PIPELINEAPPLICATIONTEST_H
