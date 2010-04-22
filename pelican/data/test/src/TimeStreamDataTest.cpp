#include "pelican/data/test/TimeStreamDataTest.h"
#include "pelican/data/TimeStreamData.h"

#include <iostream>
#include <complex>

#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( TimeStreamDataTest );

TimeStreamDataTest::TimeStreamDataTest()
    : CppUnit::TestFixture()
{
}

TimeStreamDataTest::~TimeStreamDataTest()
{
}

void TimeStreamDataTest::setUp()
{
}

void TimeStreamDataTest::tearDown()
{
}


/**
 * @details
 * Tests the various accessor methods for the time stream data blob
 */
void TimeStreamDataTest::test_accessorMethods()
{
    // Use Case
    // Construct an time stream data blob and test each of the accessor methods.
    {
        // Error tolerance use for double comparisons.
        double err = 1.0e-5;

        // Check default constructor
        TimeStreamData timeData;

        // Check resize
        unsigned nTimes = 10;
        timeData.resize(nTimes);
        CPPUNIT_ASSERT_EQUAL(unsigned(nTimes), timeData.size());

        // Check clear
        timeData.clear();
        CPPUNIT_ASSERT_EQUAL(unsigned(0), timeData.size());

        // Check the constructor that takes a size.
        nTimes = 6;
        timeData = TimeStreamData(nTimes);
        CPPUNIT_ASSERT_EQUAL(unsigned(nTimes), timeData.size());

        // Check filling in some data.
        std::complex<double>* in = timeData.data();
        for (unsigned i = 0; i < nTimes; ++i) {
            in[i] = std::complex<double>(double(i)/10.0, double(i));
        }

        // Check reading the data.
        const std::complex<double>* out = timeData.data();
        for (unsigned i = 0; i < nTimes; ++i) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL(double(i)/10.0, out[i].real(), err);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(double(i), out[i].imag(), err);
        }

        // Check the start time.
        double startTime = 0.1;
        timeData.setStartTime(startTime);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(startTime, timeData.startTime(), err);

        // Check the sample delta.
        double sampleDelta = 0.22;
        timeData.setSampleDelta(sampleDelta);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(sampleDelta, timeData.sampleDelta(), err);

        // Check clearing of metadata.
        timeData.clear();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, timeData.sampleDelta(), err);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, timeData.startTime(), err);
    }
}


} // namespace pelican
