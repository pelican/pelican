#include "AstrometryFastTest.h"
#include "pelican/modules/AstrometryFast.h"
#include "pelican/utility/pelicanTimer.h"
#include "pelican/utility/constants.h"
#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( AstrometryFastTest );
// class AstrometryFastTest
AstrometryFastTest::AstrometryFastTest()
    : CppUnit::TestFixture()
{
}

AstrometryFastTest::~AstrometryFastTest()
{
}

/**
 * @details
 * Set-up routine called before each test.
 */
void AstrometryFastTest::setUp()
{
    converter = new AstrometryFast;
}

/**
 * @details
 * Clean-up routine called after each test.
 */
void AstrometryFastTest::tearDown()
{
    delete converter;
}

/**
 * @details
 * Tests conversion of equatorial to observed horizon coordinates.
 */
void AstrometryFastTest::test_icrsEquatorialToObserved()
{
    {
        // Use Case
        // Convert RA and Dec for Oxford.

        // The Julian date for CE  2010 February 24 16:40:00.0 UT is
        // JD 2455252.19444
        const double ut = 2455252.19444  -2400000.5; // UT1 as MJD.

        // Coordinates of Beta-Andromeda.
        const double az = 238.6047222 * math::deg2rad;
        const double el = 65.70194444 * math::deg2rad;
        const double ra = 17.43375 * math::deg2rad; // (J2000).
        const double dec = 35.62016667 * math::deg2rad; // (J2000).
        const double lat = 0.90338242; // Latitude of Oxford in radians.
        const double lon = -0.021991148; // Longitude of Oxford in radians.
        double a = 0, e = 0;

        // Convert.
        converter->setSiteParameters(&site, lon, lat, 0);
        converter->setCelestialParameters(&celestial, &site, ut);
        converter->icrsEquatorialToObserved(&site, &celestial, ra, dec, &a, &e);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(az, a, 0.001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(el, e, 0.001);
    }

    {
        // Use Case
        // Convert RA and Dec for Bonn.

        // The Julian date for CE  2010 February 24 16:40:00.0 UT is
        // JD 2455252.19444
        const double ut = 2455252.19444  -2400000.5; // UT1 as MJD.

        // Coordinates of Beta-Andromeda.
        const double az = 251.4480556 * math::deg2rad;
        const double el = 61.38611111 * math::deg2rad;
        const double ra = 17.43375 * math::deg2rad; // (J2000).
        const double dec = 35.62016667 * math::deg2rad; // (J2000).
        const double lat = 0.885405529; // Latitude of Bonn in radians.
        const double lon = 0.123918376; // Longitude of Bonn in radians.
        double a = 0, e = 0;

        // Convert.
        converter->setSiteParameters(&site, lon, lat, 0);
        converter->setCelestialParameters(&celestial, &site, ut);
        converter->icrsEquatorialToObserved(&site, &celestial, ra, dec, &a, &e);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(az, a, 0.001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(el, e, 0.001);
    }

    {
        // Use Case
        // Convert RA and Dec for Perth.

        // The Julian date for CE  2010 March  3 12:00:00.0 UT is
        // JD 2455259.00000
        const double ut = 2455259.00000  -2400000.5; // UT1 as MJD.

        // Coordinates of Sirius.
        // 2010 Mar 03 12:00:00.0 : ZD  15 43 48.2, Azimuth 15 40 42.2
        const double az = 15.67838889 * math::deg2rad;
        const double el = 74.26994444 * math::deg2rad;
        const double ra = 101.2871667 * math::deg2rad; // (J2000).
        const double dec = -16.71611667 * math::deg2rad; // (J2000).
        const double lat = -0.557671481; // Latitude of Perth in radians.
        const double lon = 2.022119079; // Longitude of Perth in radians.
        double a = 0, e = 0;

        // Convert.
        converter->setSiteParameters(&site, lon, lat, 0);
        converter->setCelestialParameters(&celestial, &site, ut);
        converter->icrsEquatorialToObserved(&site, &celestial, ra, dec, &a, &e);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(az, a, 0.0001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(el, e, 0.0001);
    }
}

/**
 * @details
 * Tests conversion of observed horizon to equatorial coordinates.
 */
void AstrometryFastTest::test_icrsEquatorialFromObserved()
{
    {
        // Use Case
        // Convert azimuth and elevation for Oxford.

        // The Julian date for CE  2010 February 24 16:40:00.0 UT is
        // JD 2455252.19444
        const double ut = 2455252.19444  -2400000.5; // UT1 as MJD.

        // Coordinates of Beta-Andromeda.
        const double az = 238.6047222 * math::deg2rad;
        const double el = 65.70194444 * math::deg2rad;
        const double ra = 17.43375 * math::deg2rad; // (J2000).
        const double dec = 35.62016667 * math::deg2rad; // (J2000).
        const double lat = 0.90338242; // Latitude of Oxford in radians.
        const double lon = -0.021991148; // Longitude of Oxford in radians.
        double r = 0, d = 0;

        // Convert.
        converter->setSiteParameters(&site, lon, lat, 0);
        converter->setCelestialParameters(&celestial, &site, ut);
        converter->icrsEquatorialFromObserved(&site, &celestial,
                az, el, &r, &d);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ra, r, 0.001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(dec, d, 0.001);
    }

    {
        // Use Case
        // Convert azimuth and elevation for Bonn.

        // The Julian date for CE  2010 February 24 16:40:00.0 UT is
        // JD 2455252.19444
        const double ut = 2455252.19444  -2400000.5; // UT1 as MJD.

        // Coordinates of Beta-Andromeda.
        const double az = 251.4480556 * math::deg2rad;
        const double el = 61.38611111 * math::deg2rad;
        const double ra = 17.43375 * math::deg2rad; // (J2000).
        const double dec = 35.62016667 * math::deg2rad; // (J2000).
        const double lat = 0.885405529; // Latitude of Bonn in radians.
        const double lon = 0.123918376; // Longitude of Bonn in radians.
        double r = 0, d = 0;

        // Convert.
        converter->setSiteParameters(&site, lon, lat, 0);
        converter->setCelestialParameters(&celestial, &site, ut);
        converter->icrsEquatorialFromObserved(&site, &celestial, az, el, &r, &d);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ra, r, 0.001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(dec, d, 0.001);
    }

    {
        // Use Case
        // Convert azimuth and elevation for Perth.

        // The Julian date for CE  2010 March  3 12:00:00.0 UT is
        // JD 2455259.00000
        const double ut = 2455259.00000  -2400000.5; // UT1 as MJD.

        // Coordinates of Sirius.
        // 2010 Mar 03 12:00:00.0 : ZD  15 43 48.2, Azimuth 15 40 42.2
        const double az = 15.67838889 * math::deg2rad;
        const double el = 74.26994444 * math::deg2rad;
        const double ra = 101.2871667 * math::deg2rad; // (J2000).
        const double dec = -16.71611667 * math::deg2rad; // (J2000).
        const double lat = -0.557671481; // Latitude of Perth in radians.
        const double lon = 2.022119079; // Longitude of Perth in radians.
        double r = 0, d = 0;

        // Convert.
        converter->setSiteParameters(&site, lon, lat, 0);
        converter->setCelestialParameters(&celestial, &site, ut);
        converter->icrsEquatorialFromObserved(&site, &celestial, az, el, &r, &d);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ra, r, 0.0001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(dec, d, 0.0001);
    }
}

} // namespace pelican
