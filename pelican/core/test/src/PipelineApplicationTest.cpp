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

#include "PipelineApplicationTest.h"
#include "core/PipelineApplication.h"

#include <QtCore/QCoreApplication>


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( PipelineApplicationTest );

/**
 * @details
 * PipelineApplicationTest constructor.
 */
PipelineApplicationTest::PipelineApplicationTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details
 * PipelineApplicationTest destructor.
 */
PipelineApplicationTest::~PipelineApplicationTest()
{
}

/**
 * @details
 * Set-up routine called before each test.
 */
void PipelineApplicationTest::setUp()
{
}

/**
 * @details
 * Clean-up routine called after each test.
 */
void PipelineApplicationTest::tearDown()
{
}

/**
 * @details
 * Try to create a PipelineApplication before a QCoreApplication.
 * Expect an exception.
 */
void PipelineApplicationTest::test_create_beforeQCoreApplication()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};

    CPPUNIT_ASSERT_THROW(PipelineApplication(argc, argv), QString);
}

/**
 * @details
 * Try to create a PipelineApplication after a QCoreApplication.
 * Expect no exceptions.
 */
void PipelineApplicationTest::test_create_afterQCoreApplication()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};

    QCoreApplication app(argc, argv);
    CPPUNIT_ASSERT_NO_THROW(PipelineApplication(argc, argv));
}

/**
 * @details
 * Try to create a PipelineApplication with a configuration file name.
 * Expect no exceptions.
 */
void PipelineApplicationTest::test_commandLine_config()
{
    int argc = 2;
    char *argv[] = {(char*)"pelican", (char*)"--config=settings.xml"};

    QCoreApplication app(argc, argv);
    CPPUNIT_ASSERT_NO_THROW(PipelineApplication(argc, argv));
}

/**
 * @details
 * Try to create a PipelineApplication with --help on the command line.
 * Expect no exceptions.
 */
void PipelineApplicationTest::test_commandLine_help()
{
    int argc = 2;
    char *argv[] = {(char*)"pelican", (char*)"--help"};

    QCoreApplication app(argc, argv);
    CPPUNIT_ASSERT_NO_THROW(PipelineApplication(argc, argv));
}

/**
 * @details
 * Try to create a PipelineApplication and register an empty pipeline.
 * Expect an exception.
 */
void PipelineApplicationTest::test_registerPipeline_EmptyPipeline()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};

    QCoreApplication app(argc, argv);
    PipelineApplication *pApp = NULL;
    CPPUNIT_ASSERT_NO_THROW(pApp = new PipelineApplication(argc, argv));

    // TODO register the empty pipeline.

    delete pApp;
}

/**
 * @details
 * Try to create a PipelineApplication, register a pipeline and start it.
 */
void PipelineApplicationTest::test_start()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};

    QCoreApplication app(argc, argv);
    PipelineApplication *pApp = NULL;
    CPPUNIT_ASSERT_NO_THROW(pApp = new PipelineApplication(argc, argv));

    // TODO register a pipeline and start it.

    delete pApp;
}

} // namespace pelican
