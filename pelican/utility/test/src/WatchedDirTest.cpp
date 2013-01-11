/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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
