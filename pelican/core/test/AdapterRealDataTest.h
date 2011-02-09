#ifndef ADAPTERREALDATATEST_H
#define ADAPTERREALDATATEST_H

/**
 * @file AdapterRealDataTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

class QCoreApplication;

namespace pelican {

/**
 * @ingroup t_core
 *
 * @class AdapterRealDataTest
 *
 * @brief
 *
 * @details
 */

class AdapterRealDataTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( AdapterRealDataTest );
        CPPUNIT_TEST(test_deserialise_buffer);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_deserialise_buffer();

    public:
        AdapterRealDataTest();
        ~AdapterRealDataTest();

    private:
        QCoreApplication *_app;
};

} // namespace pelican

#endif // ADAPTERREALDATATEST_H
