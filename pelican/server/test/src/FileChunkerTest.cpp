#include "FileChunkerTest.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QTemporaryFile>
#include <QtTest/QSignalSpy>
#include <QtCore/QDebug>

#include "FileChunker.h"
#include "server/LockedData.h"
#include "server/test/ChunkerTester.h"

#include <unistd.h>

namespace pelican {

using test::ChunkerTester;

CPPUNIT_TEST_SUITE_REGISTRATION( FileChunkerTest );

/**
 * @details Constructs a FileChunkerTest object.
 */
FileChunkerTest::FileChunkerTest()
    : CppUnit::TestFixture(), _app(0), _temp(0), _msg("hello")
{
}

/**
 * @details Destroys the FileChunkerTest object.
 */
FileChunkerTest::~FileChunkerTest()
{
}


void FileChunkerTest::setUp()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _app = new QCoreApplication(argc, argv);

    // create a temporary file to perform tests on
    _temp = new QTemporaryFile;
    CPPUNIT_ASSERT( _temp->open() );
    _updateFile();

    _testFile = _temp->fileName();
    CPPUNIT_ASSERT( _testFile != "" );
}

void FileChunkerTest::tearDown()
{
    delete _temp;
    delete _app;
}

void FileChunkerTest::test_startup()
{
    // Use case:
    //   Start the Chunker with an exiting file as its target
    // Expect:
    //   A memory request to be made, and filled with the initial content
    try {
        ChunkerTester tester("FileChunker", 10*_msg.size(),
                QString("<FileChunker file=\"") + _testFile + "\" />", false);
        CPPUNIT_ASSERT_EQUAL(1, tester.writeRequestCount());
        LockedData ldata = tester.getData();
        CPPUNIT_ASSERT(ldata.isValid());
    }
    catch (const QString& msg) {
        CPPUNIT_FAIL(msg.toStdString());
    }
}

void FileChunkerTest::test_update()
{
    // Use case:
    //   Update the file with some data.
    // Expect:
    //   A memory request to be made, and filled with the new content
    try {
        QString xml =
                "<FileChunker file=\"" + _testFile + "\">"
                "   <data type=\"fileDataType\" />"
                "</FileChunker>";
        ChunkerTester tester("FileChunker", 100*_msg.size(), xml);

#ifndef __APPLE__
        usleep(1000); // FIXME This sleep really shouldn't be needed!
        QSignalSpy spy(tester.getCurrentDevice(), SIGNAL(readyRead()));
        CPPUNIT_ASSERT_EQUAL(1, tester.writeRequestCount());

        QString moredata("moredata");
        _updateFile(moredata);

        // While one might expect only one signal to be emitted it would seem
        // that this is OS dependent, with OS X often emitting two signals
        // see:
        // http://www.mail-archive.com/interest@qt-project.org/msg02987.html
        //
        // Note: when spy.count() == 2 the write request count is 3...

        CPPUNIT_ASSERT_EQUAL(1, spy.count());
        CPPUNIT_ASSERT_EQUAL(2, tester.writeRequestCount());
#endif
    }
    catch( const QString& msg)
    {
        CPPUNIT_FAIL( msg.toStdString() );
    }
}

void FileChunkerTest::_updateFile(const QString& data)
{
    QByteArray msg;
    if( data == "" )
        msg.append( _msg );
    else
        msg.append( data );
    CPPUNIT_ASSERT(_temp->write(msg.data(), msg.size()) != -1);
    CPPUNIT_ASSERT(_temp->flush());
    while (_temp->bytesToWrite() > 0)
        _temp->waitForBytesWritten(-1);
    _app->processEvents();
    CPPUNIT_ASSERT(_temp->flush());
    fsync(_temp->handle());
//    _app->flush(); // Flushes the platform specific event queues
    _app->processEvents(QEventLoop::WaitForMoreEvents, 10000);

}

} // namespace pelican
