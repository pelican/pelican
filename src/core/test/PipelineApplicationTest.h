#ifndef PIPELINEAPPLICATIONTEST_H
#define PIPELINEAPPLICATIONTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file PipelineApplicationTest.h
 */

/**
 * @class PipelineApplicationTest
 *  
 * @brief
 * 
 * @details
 * 
 */

namespace pelican {

class PipelineApplicationTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( PipelineApplicationTest );
        CPPUNIT_TEST( test_createApplication);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_createApplication();

    public:
        PipelineApplicationTest(  );
        ~PipelineApplicationTest();

    private:
};

} // namespace pelican
#endif // PIPELINEAPPLICATIONTEST_H 
