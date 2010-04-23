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
        unsigned nSubbands = 3;
        unsigned nPolarisations = 2;
        unsigned nSamples = 10;
        timeData.resize(nSubbands, nPolarisations, nSamples);
        CPPUNIT_ASSERT_EQUAL(nSubbands * nPolarisations * nSamples, timeData.size());

        // Check clear
        timeData.clear();
        CPPUNIT_ASSERT_EQUAL(unsigned(0), timeData.size());

        // Check the constructor that takes a size.
        nSamples = 6;
        timeData = TimeStreamData(nSubbands, nPolarisations, nSamples);
        CPPUNIT_ASSERT_EQUAL(nSubbands * nPolarisations * nSamples, timeData.size());

        // Check dimension return methods.
        CPPUNIT_ASSERT_EQUAL(nSubbands, timeData.nSubbands());
        CPPUNIT_ASSERT_EQUAL(nPolarisations, timeData.nPolarisations());
        CPPUNIT_ASSERT_EQUAL(nSamples, timeData.nSamples());

        // Check the start time.
        double startTime = 0.1;
        timeData.setStartTime(startTime);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(startTime, timeData.startTime(), err);

        // Check the sample delta.
        double sampleDelta = 0.22;
        timeData.setSampleDelta(sampleDelta);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(sampleDelta, timeData.sampleDelta(), err);

        // Check that the data pointer is not null when the data blob is sized.
        CPPUNIT_ASSERT(timeData.data() != NULL);

        // Check clearing of metadata.
        timeData.clear();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, timeData.sampleDelta(), err);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, timeData.startTime(), err);
        CPPUNIT_ASSERT_EQUAL(unsigned(0), timeData.nSubbands());
        CPPUNIT_ASSERT_EQUAL(unsigned(0), timeData.nPolarisations());
        CPPUNIT_ASSERT_EQUAL(unsigned(0), timeData.nSamples());

        // Check that the data pointer is null when the data blob is empty.
        CPPUNIT_ASSERT(timeData.data() == NULL);

        // Resize and add some data.
        timeData.resize(nSubbands, nPolarisations, nSamples);
        std::complex<double>* in = timeData.data();
        for (unsigned i = 0, c = 0; c < nSubbands; ++c) {
            for (unsigned p = 0; p < nPolarisations; ++p) {
                for (unsigned s = 0; s < nSamples; ++s) {
                    double re = double(c + p + s);
                    double im = double(s - p - c);
                    in[i] = std::complex<double>(re, im);
                    i++;
                }
            }
        }

        const std::complex<double>* out = timeData.data();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, out[0].real(), err);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, out[0].imag(), err);
        unsigned sb = 1; // sub-band
        unsigned p = 0; // polarisation
        unsigned s = 2; // sample
        unsigned index =  sb * nPolarisations * nSamples + p * nSamples + s;
        CPPUNIT_ASSERT_EQUAL(index, timeData.index(sb, p, s));
        CPPUNIT_ASSERT(index < timeData.size());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(double(sb + p + s), out[index].real(), err);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(double(s - p - sb), out[index].imag(), err);

        sb = 1; p = 0; s = 0;
        // Check pointer to sub-band
        const std::complex<double>* sb1 = timeData.data(sb);
        CPPUNIT_ASSERT(sb1 == &out[timeData.index(sb, p, s)]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(double(sb + p + s), sb1[0].real(), err);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(double(s - p - sb), sb1[0].imag(), err);

        sb = 2; p = 1; s = 0;
        const std::complex<double>* sb2p1 = timeData.data(sb, p);
        CPPUNIT_ASSERT(sb2p1 == &out[timeData.index(sb, p, s)]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(double(sb + p + s), sb2p1[0].real(), err);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(double(s - p - sb), sb2p1[0].imag(), err);

        s = 3;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(double(sb + p + s), sb2p1[s].real(), err);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(double(s - p - sb), sb2p1[s].imag(), err);
    }
}


} // namespace pelican
