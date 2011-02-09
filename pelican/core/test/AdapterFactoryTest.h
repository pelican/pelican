#ifndef ADAPTERFACTORYTEST_H
#define ADAPTERFACTORYTEST_H

/**
 * @file AdapterFactoryTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_core
 *
 * @class AdatperFactoryTest
 *
 * @brief
 * CppUnit tests for AdapterFactory class.
 *
 * @details
 * Tries to create a number of modules.
 */
class AdapterFactoryTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ModuleFactoryTest );
        CPPUNIT_TEST( test_create );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_create();

    public:
        AdapterFactoryTest();
        ~AdapterFactoryTest();

    private:
};

} // namespace pelican

#endif // ADAPTERFACTORYTEST_H
