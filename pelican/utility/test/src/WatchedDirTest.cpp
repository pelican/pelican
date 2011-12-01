#include "WatchedDirTest.h"
#include "WatchedDir.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QTemporaryFile>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtTest/QSignalSpy>


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( WatchedDirTest );
/**
 *@details WatchedDirTest 
 */
WatchedDirTest::WatchedDirTest()
    : CppUnit::TestFixture()
{
    _msg = "hello\n";
}

/**
 *@details
 */
WatchedDirTest::~WatchedDirTest()
{
}

void WatchedDirTest::setUp()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _app = new QCoreApplication(argc, argv);

    QString dirname = QDir::tempPath() + "/WatchedDirTest_" 
                        + QString().setNum(QCoreApplication::applicationPid());
    _tempDir.setPath( dirname );
    if( ! _tempDir.exists() ) {
        CPPUNIT_ASSERT(_tempDir.mkpath( dirname ));
    }
}

void WatchedDirTest::tearDown()
{
     CPPUNIT_ASSERT( _tempDir.rmpath( _tempDir.absolutePath() ));
     delete _app;
}

void WatchedDirTest::test_method()
{
    QString dirname = _tempDir.absolutePath();
    {
        // Use Case:
        // Empty directory watched
        // Expect readyRead on any new file
        WatchedDir wd(dirname);
        QSignalSpy spy( &wd, SIGNAL( readyRead() ) );
        boost::shared_ptr<QTemporaryFile> file1=addFile(dirname);
        _app->processEvents();
        CPPUNIT_ASSERT_EQUAL( 1, spy.count() );
        CPPUNIT_ASSERT_EQUAL( (int)file1->size() , (int)wd.bytesAvailable() );
        boost::shared_ptr<QTemporaryFile> file2=addFile(dirname);
        _app->processEvents();
        // ensure we can read the data
        // and that the stream at end markers are set only when all files
        // are consumed from the stream
        CPPUNIT_ASSERT_EQUAL( file1->fileName().toStdString(), wd.fileName().toStdString() );
        QByteArray f1 = wd.readFile();
        CPPUNIT_ASSERT( ! wd.atEnd() );
        CPPUNIT_ASSERT_EQUAL( (int)file1->size(), f1.size() );
        CPPUNIT_ASSERT_EQUAL( file2->fileName().toStdString(), wd.fileName().toStdString());
        CPPUNIT_ASSERT_EQUAL( 2, spy.count() ); // emit signal each time we switch files
        CPPUNIT_ASSERT_EQUAL( (int)file2->size() , (int)wd.bytesAvailable() );
        QByteArray f2 = wd.readFile();
        CPPUNIT_ASSERT_EQUAL( (int)file2->size(), (int)f2.size() );
        CPPUNIT_ASSERT_EQUAL( 2, spy.count() );
        CPPUNIT_ASSERT( wd.atEnd() );
    }
}

boost::shared_ptr<QTemporaryFile> WatchedDirTest::addFile( const QString& dir ) const {
    boost::shared_ptr<QTemporaryFile> t( new QTemporaryFile );
    t->setFileTemplate( dir + "/testFile" );
    t->open();
    CPPUNIT_ASSERT( t->write(_msg.data(), _msg.size() ) != -1 );
    CPPUNIT_ASSERT( t->flush());
    fsync(t->handle());
    t->close();
    return t;
}

} // namespace pelican
