#ifndef CONFIGTEST_H
#define CONFIGTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ConfigTest.h
 */

/**
 * @class ConfigTest
 *  
 * @brief
 * Class to test the configuration object.
 * 
 * @details
 * QApplication exists for Qt garbage collection from QFile.
 */

class QCoreApplication;

namespace pelican {

class Config;

class ConfigTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ConfigTest );
        CPPUNIT_TEST( test_noConfigFile );
        CPPUNIT_TEST( test_emptyConfigFile );
//        CPPUNIT_TEST( test_invalidConfigFile );
//        CPPUNIT_TEST( test_readConfigFile );
//        CPPUNIT_TEST( test_getModuleConfig() );
//        CPPUNIT_TEST( test_getServerConfig() );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_noConfigFile();
        void test_emptyConfigFile();
        void test_invalidConfigFile();
        void test_readConfigFile();
        void test_getModuleConfig();
        void test_getServerConfig();

    public:
        ConfigTest();
        ~ConfigTest();

    private:
        QCoreApplication *app;
};

} // namespace pelican
#endif // CONFIGTEST_H 
