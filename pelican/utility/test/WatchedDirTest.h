#ifndef WATCHEDDIRTEST_H
#define WATCHEDDIRTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <QtCore/QDir>
#include <QtCore/QTemporaryFile>
#include <boost/shared_ptr.hpp>
class QCoreApplication;

/**
 * @file WatchedDirTest.h
 */

namespace pelican {

/**
 * @ingroup t_utility
 *
 * @class WatchedDirTest
 *  
 * @brief
 *    Unit test for the WatchedDir class
 * @details
 * 
 */

class WatchedDirTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( WatchedDirTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        WatchedDirTest(  );
        ~WatchedDirTest();

    protected:
        boost::shared_ptr<QTemporaryFile> addFile( const QString& dir ) const;

    private:
        QCoreApplication* _app;
        QDir _tempDir;
        QByteArray _msg;
};

} // namespace pelican
#endif // WATCHEDDIRTEST_H 
