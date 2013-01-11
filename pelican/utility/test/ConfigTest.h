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

#ifndef CONFIGTEST_H
#define CONFIGTEST_H

/**
 * @file ConfigTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

/**
 * @ingroup t_utility
 *
 * @class ConfigTest
 *
 * @brief
 * Class to test the configuration object.
 *
 * @details
 *
 * \note Created as a QCoreApplication for Qt garbage collection.
 */

class QCoreApplication;

namespace pelican {

class ConfigTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ConfigTest );
        CPPUNIT_TEST( test_emptyFileName );
        CPPUNIT_TEST( test_fileDoesntExist );
        CPPUNIT_TEST( test_invalidDocType );
        CPPUNIT_TEST( test_setConfiguration );
        CPPUNIT_TEST( test_getConfiguration );
        CPPUNIT_TEST( test_configFileRead );
        CPPUNIT_TEST( test_configFromString );
        CPPUNIT_TEST( test_testConfig );
        CPPUNIT_TEST( test_searchFile );
        CPPUNIT_TEST( test_preprocess );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        /// Test for an empty configuration file name string.
        void test_emptyFileName();
        /// Test for a configuration file that doesn't exist.
        void test_fileDoesntExist();
        /// Test for an invalid configuration file.
        void test_invalidDocType();
        /// Test for setting configuration options.
        void test_setConfiguration();
        /// Test for returning configuration options
        void test_getConfiguration();
        /// Test for reading a XML config from file
        void test_configFileRead();
        /// Test creating configuration from a QString
        void test_configFromString();
        /// Test using the TestConfig class.
        void test_testConfig();
        /// Test filename searching
        void test_searchFile();
        /// Test preprocessing the document.
        void test_preprocess();

    public:
        ConfigTest();
        ~ConfigTest();

    private:
        QCoreApplication *app;
};
} // namespace pelican

#endif // CONFIGTEST_H
