#ifndef FILECHUNKERTEST_H
#define FILECHUNKERTEST_H

/**
 * @file FileChunkerTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
#include <QtCore/QString>
class QCoreApplication;
class QTemporaryFile;

namespace pelican {

/**
 * @ingroup t_server
 *
 * @class FileChunkerTest
 *
 * @brief
 *    unit test for the fileChunker
 * @details
 *
 */
class FileChunkerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( FileChunkerTest );
        CPPUNIT_TEST( test_startup );
        CPPUNIT_TEST( test_update );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_startup();
        void test_update();

    public:
        /// FileChunkerTest constructor.
        FileChunkerTest();

        /// FileChunkerTest destructor.
        ~FileChunkerTest();

    protected:
        void _updateFile(const QString& data = QString() );

    private:
        QCoreApplication* _app;
        QTemporaryFile* _temp;
        QString _testFile;
        QString _msg;

};

} // namespace pelican

#endif // FILECHUNKERTEST_H
