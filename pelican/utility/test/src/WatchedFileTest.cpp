#include <unistd.h>
#include "WatchedFileTest.h"
#include <QtTest/QSignalSpy>
#include <QtCore/QTemporaryFile>
#include <QtCore/QCoreApplication>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QStringList>

#include "pelican/utility/WatchedFile.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( WatchedFileTest );

/**
 * @details Constructs a WatchedFileTest object.
 */
WatchedFileTest::WatchedFileTest()
    : CppUnit::TestFixture(), _app(0), _temp(0), _msg(0)
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
    CPPUNIT_ASSERT(_temp->open());
    _updateFile();
}

void WatchedFileTest::tearDown()
{
    delete _temp;
    delete _app;
}


void WatchedFileTest::test_QFileSystemWatcher()
{
    // Use case:
    // Test the number of times the QWatchedFile::fileChanged() signal
    // is emitted when a watched file is modified.
    //
    // Expect:
    // While one might expect only one signal to be emitted it would seem that
    // this is a little OS dependent with OS X often emitting two signals at
    // least according to:
    // http://www.mail-archive.com/interest@qt-project.org/msg02987.html
    //
    // NOTE Adding a pre-processor guard therefore avoids the issue for OS X
    // in these tests, however anyone using pelcan::WatchedFile class will
    // have to be very careful.

    QTemporaryFile temp;
    temp.open();
    QString filename = temp.fileName();
    CPPUNIT_ASSERT( filename != "" );

    QFileSystemWatcher watcher;
    watcher.addPath(filename);
    CPPUNIT_ASSERT_EQUAL(1, watcher.files().count());
    QSignalSpy spy(&watcher, SIGNAL(fileChanged(QString)));
    _app->processEvents();
    CPPUNIT_ASSERT_EQUAL(0, spy.count());
    temp.write(_msg.data());
    temp.flush();
    fsync(temp.handle());
    _app->processEvents();
#if !(defined(__APPLE__) && defined(__MACH__))
        CPPUNIT_ASSERT_EQUAL(1, spy.count());
#else
        CPPUNIT_ASSERT(spy.count() >= 1 && spy.count() < 3);
#endif
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
        QSignalSpy spy(&wf, SIGNAL(readyRead()));
        CPPUNIT_ASSERT(wf.open(QIODevice::ReadOnly));
        _updateFile();
        _app->processEvents();
#if !(defined(__APPLE__) && defined(__MACH__))
        CPPUNIT_ASSERT_EQUAL(1, spy.count());
#else
        CPPUNIT_ASSERT(spy.count() >= 1);
#endif
    }
    {
        // Use Case:
        // Filename passed in constructor
        // expect:
        // readReady to be emitted when file is updated
        WatchedFile wf(filename);
        QSignalSpy spy(&wf, SIGNAL(readyRead()));
        CPPUNIT_ASSERT(wf.open(QIODevice::ReadOnly));
        _app->processEvents();
        _updateFile();
        _app->processEvents();
#if !(defined(__APPLE__) && defined(__MACH__))
        CPPUNIT_ASSERT_EQUAL(1, spy.count());
#else
        CPPUNIT_ASSERT(spy.count() >= 1);
#endif
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
