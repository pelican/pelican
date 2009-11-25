#ifndef MODULEFACTORYTEST_H
#define MODULEFACTORYTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ModuleFactoryTest.h
 */

namespace pelican {

/**
 * @class ModuleFactoryTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class ModuleFactoryTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ModuleFactoryTest );
        CPPUNIT_TEST( test_createTestModule );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_createTestModule();

    public:
        ModuleFactoryTest(  );
        ~ModuleFactoryTest();

    private:
};

} // namespace pelican
#endif // MODULEFACTORYTEST_H 
