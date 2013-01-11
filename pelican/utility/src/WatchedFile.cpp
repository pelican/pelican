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

#include "utility/WatchedFile.h"
#include <QtCore/QtGlobal>

namespace pelican {


/**
 * @details Constructs a WatchedFile object.
 */
WatchedFile::WatchedFile(const QString& name, QObject* parent)
    : QFile(name, parent)
{
    _init();
}

WatchedFile::WatchedFile(QObject* parent)
    : QFile( parent)
{
    _init();
}

void WatchedFile::_init()
{
    connect( &_watcher, SIGNAL( fileChanged(const QString&) ),
             this,  SLOT(_fileChanged(const QString &) ),
                                  Qt::DirectConnection);
    if( fileName() != QString() )
    {
        _watcher.addPath( fileName() );
    }
}

/**
 * @details Destroys the WatchedFile object.
 */
WatchedFile::~WatchedFile()
{
}

void WatchedFile::_fileChanged(const QString& )
{
    emit readyRead();
}

void WatchedFile::setFileName(const QString& name)
{
    if( fileName() != QString() )
        _watcher.removePath( fileName() );
    QFile::setFileName(name);
    _watcher.addPath( fileName() );
}

} // namespace pelican
