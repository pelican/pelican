#ifndef CONFIGNODETEST_H
#define CONFIGNODETEST_H

/**
 * @file ConfigNodeTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

/**
 * @ingroup t_utility
 *
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
        CPPUNIT_TEST( test_setFromString );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_methods();
        void test_setFromString();

    public:
        ConfigNodeTest();
        ~ConfigNodeTest();

    private:
        QCoreApplication *app;
};

} // namespace pelican

#endif // CONFIGNODETEST_H
