#include "TestFile.h"
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QMutexLocker>
#include <QtCore/QCoreApplication>
#include <iostream>

namespace pelican {
namespace test {

long TestFile::_id = 0;
QList<QString> TestFile::_reserved = QList<QString>();

/**
 * @details Constructs a TestFile object.
 */
TestFile::TestFile( bool deleteFile )
     : _cleanup(deleteFile)
{
}

/**
 * @details Destroys the TestFile object.
 */
TestFile::~TestFile()
{
    if( _cleanup ) {
        if( QFile::exists(_filename) ) {
            QFile::remove(_filename);
        }
        QMutexLocker lock( &_mutex );
        _reserved.removeAll(_filename);
    }
    else {
        if( QFileInfo(_filename).isDir() ) {
            std::cout << "TestFile: not removing temporary file: "
                      << _filename.toStdString() << std::endl;
        }
    }
}

QString TestFile::filename()
{
    if( _filename == "" ) {
        QMutexLocker lock( &_mutex );
        do {
             _filename = QDir::tempPath() + "/TestFile_"
                                + QString().setNum(QCoreApplication::applicationPid()) + "_"
                                + QString().setNum(++_id) + ".tmp";
        }
        while( QFile::exists(_filename) && _reserved.contains(_filename) );
        _reserved.append(_filename);
    }
    return _filename;
}

} // namespace test
} // namespace pelican
