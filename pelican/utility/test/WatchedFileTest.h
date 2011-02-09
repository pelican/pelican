#ifndef WATCHEDFILETEST_H
#define WATCHEDFILETEST_H

/**
 * @file WatchedFileTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
#include <QtCore/QByteArray>

class QCoreApplication;
class QTemporaryFile;

namespace pelican {

/**
 * @ingroup t_utility
 *
 * @class WatchedFileTest
 *
 * @brief
 * Unit test for the WatchedFile class
 *
 * @details
 */

class WatchedFileTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( WatchedFileTest );
        CPPUNIT_TEST( test_watch );
        CPPUNIT_TEST( test_nameChange );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_watch();
        void test_nameChange();

    public:
        /// WatchedFileTest constructor.
        WatchedFileTest();

        /// WatchedFileTest destructor.
        ~WatchedFileTest();

    protected:
        void _updateFile();

    private:
        QCoreApplication* _app;
        QTemporaryFile* _temp;
        QByteArray _msg;
};

} // namespace pelican
#endif // WATCHEDFILETEST_H
