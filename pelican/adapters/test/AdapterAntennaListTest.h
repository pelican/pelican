#ifndef ADAPTERANTENNALISTTEST_H
#define ADAPTERANTENNALISTTEST_H

#include <cppunit/extensions/HelperMacros.h>

class QCoreApplication;

/**
 * @file AdapterAntennaListTest.h
 */

namespace pelican {

/**
 * @class AdapterAntennaListTest
 *
 * @brief
 *
 * @details
 */

class AdapterAntennaListTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( AdapterAntennaListTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        AdapterAntennaListTest( );
        ~AdapterAntennaListTest();

    private:
        QCoreApplication *_app;
};

} // namespace pelican

#endif // ADAPTERANTENNALISTTEST_H
