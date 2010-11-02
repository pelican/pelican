#ifndef MODULEFACTORYTEST_H
#define MODULEFACTORYTEST_H

/**
 * @file ModuleFactoryTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_core
 *
 * @class ModuleFactoryTest
 *
 * @brief
 * CppUnit tests for ModuleFactory class.
 *
 * @details
 * Tries to create a number of modules.
 */
class ModuleFactoryTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ModuleFactoryTest );
        CPPUNIT_TEST( test_createModule_EmptyModule );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_createModule_EmptyModule();

    public:
        ModuleFactoryTest(  );
        ~ModuleFactoryTest();

    private:
};

} // namespace pelican

#endif // MODULEFACTORYTEST_H
