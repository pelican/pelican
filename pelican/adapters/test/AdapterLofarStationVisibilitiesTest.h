#ifndef ADAPTERLOFARSTATIONVISIBILITIESTEST_H
#define ADAPTERLOFARSTATIONVISIBILITIESTEST_H

#include <cppunit/extensions/HelperMacros.h>

class QCoreApplication;

/**
 * @file AdapterLofarStationVisibilitiesTest.h
 */

namespace pelican {

class VisGen;

/**
 * @class AdapterLofarStationVisibilitiesTest
 *
 * @brief
 *
 * @details
 *
 */

class AdapterLofarStationVisibilitiesTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( AdapterLofarStationVisibilitiesTest );
        CPPUNIT_TEST(test_deserialise_buffer);
        CPPUNIT_TEST(test_deserialise_file);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_deserialise_buffer();
        void test_deserialise_file();

    public:
        AdapterLofarStationVisibilitiesTest();
        ~AdapterLofarStationVisibilitiesTest();

    private:
        // Returns the expected data value at this location.
        double _dataVal(unsigned ai, unsigned aj, unsigned c, unsigned p) {
            return p * 1000.0 + c + ai/100.0 + aj/100.0;
        }

        QCoreApplication *_app;
};

} // namespace pelican
#endif // ADAPTERLOFARSTATIONVISIBILITIESTEST_H
