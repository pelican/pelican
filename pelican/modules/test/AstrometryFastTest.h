#ifndef ASTROMETRYFASTTEST_H
#define ASTROMETRYFASTTEST_H

#include "pelican/modules/AstrometryFast.h"

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file AstrometryFastTest.h
 */

namespace pelican {

/**
 * @class AstrometryFastTest
 *
 * @brief
 * Tests various functions of the astrometry module.
 *
 * @details
 * Performs unit tests on the astrometric transform module
 * using the CppUnit framework.
 */
class AstrometryFastTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( AstrometryFastTest );
        CPPUNIT_TEST( test_icrsEquatorialToObserved );
        CPPUNIT_TEST( test_icrsEquatorialFromObserved );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_icrsEquatorialToObserved();
        void test_icrsEquatorialFromObserved();

    public:
        AstrometryFastTest(  );
        ~AstrometryFastTest();

    private:
        AstrometryFast *converter;
        SiteData site;
        CelestialData celestial;
};

} // namespace pelican

#endif // ASTROMETRYFASTTEST_H
