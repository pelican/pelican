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
        CPPUNIT_TEST(test_method);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        AdapterLofarStationVisibilitiesTest();
        ~AdapterLofarStationVisibilitiesTest();

    private:
        QCoreApplication *_app;
};

} // namespace pelican
#endif // ADAPTERLOFARSTATIONVISIBILITIESTEST_H
