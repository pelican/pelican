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
