#ifndef OUTPUTSTREAMMANAGERTEST_H
#define OUTPUTSTREAMMANAGERTEST_H

/**
 * @file OutputStreamManagerTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
#include "pelican/utility/Config.h"

namespace pelican {
class OutputStreamManager;

/**
 * @ingroup t_output
 *
 * @class OutputStreamManagerTest
 *
 * @brief
 * Unit test for the OutputStreamManager
 *
 * @details
 */

class OutputStreamManagerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( OutputStreamManagerTest );
        CPPUNIT_TEST( test_connectToStream );
        CPPUNIT_TEST( test_configuration );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_connectToStream();
        void test_configuration();


    protected:
        OutputStreamManager* _getManager(const QString& xml = "", const QString& base = "" );

    public:
        OutputStreamManagerTest(  );
        ~OutputStreamManagerTest();

    private:
        Config* _config;
        Config::TreeAddress _address;
};

} // namespace pelican
#endif // OUTPUTSTREAMMANAGERTEST_H
