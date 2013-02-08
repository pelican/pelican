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

#ifndef WATCHEDFILE_H
#define WATCHEDFILE_H

/**
 * @file WatchedFile.h
 */

#include <QtCore/QFile>
#include <QtCore/QFileSystemWatcher>

namespace pelican {

/**
 * @ingroup c_utility
 *
 * @class WatchedFile
 *
 * @brief
 * A QFile that emits the readyRead() signal whenever it is updated
 *
 * @details
 */

class WatchedFile : public QFile
{
    Q_OBJECT

    public:
        /// WatchedFile constructor.
        WatchedFile( const QString& name, QObject* parent = 0 );
        WatchedFile( QObject* parent = 0 );

        /// WatchedFile destructor.
        ~WatchedFile();

        /// overriden setFileName operator
        void setFileName ( const QString & name );

    protected:
        void _init();

    private slots:
        void _fileChanged(const QString&);

    private:
         QFileSystemWatcher _watcher;
};

} // namespace pelican
#endif // WATCHEDFILE_H
