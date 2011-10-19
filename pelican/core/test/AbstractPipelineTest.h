#ifndef ABSTRACTPIPELINETEST_H
#define ABSTRACTPIPELINETEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file AbstractPipelineTest.h
 */

namespace pelican {

/**
 * @class AbstractPipelineTest
 *   unit test for the base functions of the Abstract Pipeline
 * @brief
 * 
 * @details
 * 
 */

class AbstractPipelineTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( AbstractPipelineTest );
        CPPUNIT_TEST( test_createBlobs );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_createBlobs();

    public:
        AbstractPipelineTest(  );
        ~AbstractPipelineTest();

    private:
};

} // namespace pelican
#endif // ABSTRACTPIPELINETEST_H 
