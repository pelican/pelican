#include "WatchedFileTest.h"
#include <QtTest/QSignalSpy>
#include <QtCore/QTemporaryFile>
#include <QtCore/QCoreApplication>

#include "pelican/utility/WatchedFile.h"

//#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( WatchedFileTest );
/**
 * @details Constructs a WatchedFileTest object.
 */
WatchedFileTest::WatchedFileTest()
    : CppUnit::TestFixture()
{
    _msg = "hello\n";
}

/**
 * @details Destroys the WatchedFileTest object.
 */
WatchedFileTest::~WatchedFileTest()
{
}

void WatchedFileTest::setUp()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _app = new QCoreApplication(argc, argv);

    // create a temporary file to perform tests on
    _temp = new QTemporaryFile;
    CPPUNIT_ASSERT( _temp->open() );
    _updateFile();

}

void WatchedFileTest::tearDown()
{
    delete _temp;
    delete _app;
}

void WatchedFileTest::test_watch()
{
    QString filename = _temp->fileName();
    CPPUNIT_ASSERT( filename != "" );

    {
        // Use Case:
        // Filename passed via setfileName()
        // expect:
        // readReady to be emitted when file is updated
        WatchedFile wf;
        wf.setFileName(filename);
        QSignalSpy spy( &wf, SIGNAL( readyRead() ) );
        CPPUNIT_ASSERT( wf.open( QIODevice::ReadOnly ) );
        _updateFile();
        _app->processEvents();
        CPPUNIT_ASSERT_EQUAL( 1, spy.count() );
    }
    {
        // Use Case:
        // Filename passed in constructor
        // expect:
        // readyRead() to be emitted on opening
        // readReady to be emitted when file is updated
        WatchedFile wf(filename);
        QSignalSpy spy( &wf, SIGNAL( readyRead() ) );
        CPPUNIT_ASSERT( wf.open( QIODevice::ReadOnly ) );
        _app->processEvents();
        _updateFile();
        _app->processEvents();
        CPPUNIT_ASSERT_EQUAL( 1, spy.count() );
    }
}

void WatchedFileTest::test_nameChange()
{
}

void WatchedFileTest::_updateFile()
{
    CPPUNIT_ASSERT( _temp->write(_msg.data(), _msg.size() ) != -1 );
    _app->processEvents();
    CPPUNIT_ASSERT(_temp->flush());
    fsync(_temp->handle());
}

} // namespace pelican
