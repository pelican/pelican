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
