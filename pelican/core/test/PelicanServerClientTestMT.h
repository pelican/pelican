#ifndef PELICANSERVERCLIENTTESTMT_H
#define PELICANSERVERCLIENTTESTMT_H

/**
 * @file PelicanServerClientTestMT.h
 */

#include <cppunit/extensions/HelperMacros.h>
class QCoreApplication;

namespace pelican {

/**
 * @ingroup t_core
 *
 * @class PelicanServerClientTestMT
 *
 * @brief
 *   Multi Threaded integration tests for the PelicanServer Client
 * @details
 *
 */

class PelicanServerClientTestMT : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( PelicanServerClientTestMT );
        CPPUNIT_TEST( test_getData );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_getData();

    public:
        PelicanServerClientTestMT(  );
        ~PelicanServerClientTestMT();

    private:
        QCoreApplication* _coreApp;
};

} // namespace pelican
#endif // PELICANSERVERCLIENTTESTMT_H
