#ifndef CONFIGNODETEST_H
#define CONFIGNODETEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ConfigNodeTest.h
 */

/**
 * @class ConfigTest
 *  
 * @brief
 * Class to test the configuration node object.
 * 
 * @details
 *
 * @note Created as a QCoreApplication for Qt garbage collection.
 */

class QCoreApplication;

namespace pelican {

class Config;

class ConfigNodeTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ConfigNodeTest );
        CPPUNIT_TEST( test_methods );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_methods();

    public:
        ConfigNodeTest();
        ~ConfigNodeTest();

    private:
        QCoreApplication *app;
};

} // namespace pelican

#endif // CONFIGNODETEST_H
