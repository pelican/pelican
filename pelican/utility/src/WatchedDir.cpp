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

#include "WatchedDir.h"
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QDir>
#include <QtCore/QByteArray>
#include <iostream>

namespace pelican {


/**
 *@details WatchedDir
 */
WatchedDir::WatchedDir( const QString& name, QObject* parent )
    : QFile(parent)
{
    setFileName( name );
    connect( &_watcher, SIGNAL( directoryChanged(const QString&) ),
             this,  SLOT(_dirChanged(const QString &) ),
                                  Qt::DirectConnection);
//    connect( &_watcher, SIGNAL( fileChanged(const QString&) ),
//             this,  SLOT(_fileChanged(const QString &) ),
//                                  Qt::DirectConnection);
}

/**
 *@details
 */
WatchedDir::~WatchedDir()
{
}

void WatchedDir::setFileName( const QString& name )
{
    _watcher.removePaths( _watcher.directories() );
    _watcher.addPath(name);
    _files=_watcher.files();
}

void WatchedDir::_fileChanged(const QString& file )
{
//std::cout << "_filechanged():" << file.toStdString() << std::endl;
    if( ! _streamFiles.contains( file ) ) {
        _streamFiles.append(file);
        emit readyRead();
    }
}

QByteArray WatchedDir::readFile() {
    QByteArray tmp;
    tmp.resize(int(bytesAvailable()));
    qint64 readBytes = readFile(tmp.data(), tmp.size());
    tmp.resize(readBytes < 0 ? 0 : int(readBytes));
    return tmp;
}

qint64 WatchedDir::readFile( char* data, qint64 len ) {
    //qint64 avail = bytesAvailable();
    qint64 read = QFile::read(data, len);
    //if( read > 0 && read >= avail ) {
    if( atEnd() ) {
        close();
        _openNext();
    }
    return read;
}

void WatchedDir::_openNext() {
    while( ! isOpen() && _streamFiles.size() > 0  ) {
        QFile::setFileName(_streamFiles.takeFirst());
        if( exists() ) {
            if( open(QIODevice::ReadOnly) ) {
                emit readyRead();
            }
        }
    }
}

void WatchedDir::_dirChanged(const QString& dir )
{
    int num = 0;
    QDir d(dir);
    QStringList flist = d.entryList( QDir::Files | QDir::Readable );
    // find out any new files that have been added
    foreach( const QString& file, flist ) {
       if( ! _files.removeAll( file ) ) {
           _streamFiles.append(dir + "/" + file);
           ++num;
       }
    }
    _files = flist;
    if( num ) {
       _openNext();
    }
}

} // namespace pelican
