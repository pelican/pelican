#ifndef PELICANTIMERECORDERTEST_H
#define PELICANTIMERECORDERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file PelicanTimeRecorderTest.h
 */

namespace pelican {

/**
 * @class PelicanTimeRecorderTest
 * 
 * @brief
 * 
 * @details
 * 
 */
class PelicanTimeRecorderTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( PelicanTimeRecorderTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        /// PelicanTimeRecorderTest constructor.
        PelicanTimeRecorderTest();

        /// PelicanTimeRecorderTest destructor.
        ~PelicanTimeRecorderTest();

    private:
};

} // namespace pelican

#endif // PELICANTIMERECORDERTEST_H 
