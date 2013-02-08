#include "FileChunkerTest.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QTemporaryFile>
#include <QtTest/QSignalSpy>
#include "FileChunker.h"
#include "server/LockedData.h"
#include "server/test/ChunkerTester.h"
#include <QtCore/QDebug>

namespace pelican {

using test::ChunkerTester;

CPPUNIT_TEST_SUITE_REGISTRATION( FileChunkerTest );

/**
 * @details Constructs a FileChunkerTest object.
 */
FileChunkerTest::FileChunkerTest()
    : CppUnit::TestFixture(), _msg("hello")
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
                QString("<FileChunker file=\"") + _testFile + "\" />");

        CPPUNIT_ASSERT_EQUAL( 1, tester.writeRequestCount() );
        LockedData ldata = tester.getData();
        CPPUNIT_ASSERT( ldata.isValid() );
        //CPPUNIT_ASSERT_EQUAL( data.size(), _msg.size() );
    }
    catch( const QString& msg)
    {
        CPPUNIT_FAIL( msg.toStdString() );
    }
}

void FileChunkerTest::test_update()
{
    // Use case:
    //   Update the file with some data.
    // Expect:
    //   A memory request to be made, and filled with the new content
    try {
        ChunkerTester tester("FileChunker", 100*_msg.size(),
                QString("<FileChunker file=\"") + _testFile + "\"/>");
        sleep(1);

        QSignalSpy spy(tester.getCurrentDevice(), SIGNAL(readyRead()));
        //CPPUNIT_ASSERT_EQUAL(1, tester.writeRequestCount());

        QString moredata("moredata");
        _updateFile(moredata);
        sleep(1);

        _app->processEvents();

        qDebug() << __PRETTY_FUNCTION__ << " _testFile = " <<_testFile;

        // While one might expect only one signal to be emitted it would seem that
        // this is a little OS dependent with OS X often emitting two signals at
        // least according to:
        // http://www.mail-archive.com/interest@qt-project.org/msg02987.html
        //
        // While it is possible to add a preprocessor guard to change the
        // checking on OS X the impact of this when using a FileChunker
        // is currently unknown so its better to let the test fail.
        // NOTE that when spy.count() == 2 the write request count is 3...!

        CPPUNIT_ASSERT_EQUAL(1, spy.count());
        CPPUNIT_ASSERT_EQUAL(2, tester.writeRequestCount());
        //CPPUNIT_ASSERT_EQUAL( tester->getData().size(), _testData.size()  + moredata.size());
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
    CPPUNIT_ASSERT( _temp->write(msg.data(), msg.size() ) != -1 );
    _app->processEvents();
    CPPUNIT_ASSERT(_temp->flush());
    fsync(_temp->handle());
    _app->processEvents();
}

} // namespace pelican
