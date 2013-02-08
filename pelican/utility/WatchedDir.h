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

#ifndef WATCHEDDIR_H
#define WATCHEDDIR_H


#include <QtCore/QFile>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
class QByteArray;

/**
 * @file WatchedDir.h
 */

namespace pelican {

/**
 * @class WatchedDir
 *
 * @brief
 *    A directory watcher that implemebnts the QIODevice interface
 * @details
 *
 */

class WatchedDir : public QFile
{
    Q_OBJECT

    public:
        WatchedDir( const QString& name, QObject* parent = 0 );
        ~WatchedDir();
        virtual void setFileName( const QString& name );
        //virtual bool isSequential () const { return true; }
        QByteArray readFile();
        qint64 readFile( char* data, qint64 len );

    protected:
        void _openNext();

    private slots:
        void _dirChanged(const QString&);
        void _fileChanged(const QString&);

    private:
         QFileSystemWatcher _watcher;
         QList<QString> _streamFiles; // list of changed/new files to be read
         QStringList _files; // list of files at the last dir change
};

} // namespace pelican
#endif // WATCHEDDIR_H
