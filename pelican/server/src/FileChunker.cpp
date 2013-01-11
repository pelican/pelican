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

#include "server/FileChunker.h"
#include "utility/WatchedFile.h"

namespace pelican {


/**
 * @details Constructs a FileChunker object.
 */
FileChunker::FileChunker(const ConfigNode& config)
    : AbstractChunker( config)
{
    if( config.getDomElement().isNull() )
    {
        throw( QString("FileChunker: empty config passed in constructor") );
    }
    if( ! config.hasAttribute("file") )
        throw( QString("FileChunker: no \"file\" attribute specified" ));
    _fileName = config.getAttribute("file");
    if( _fileName == "" )
        throw( QString("FileChunker: no filename specified" ));
}

/**
 * @details Destroys the FileChunker object.
 */
FileChunker::~FileChunker()
{
}

QIODevice* FileChunker::newDevice()
{
    WatchedFile* device = new WatchedFile(_fileName);
    if( ! device->open(QIODevice::ReadOnly) )
        throw(QString("FileChunker: unable to open file:%1").arg(_fileName));
    //std::cout << "Watching file " << device->fileName().toStdString() << std::endl;
    return device;
}

void FileChunker::next(QIODevice* dev)
{
    QFile* file=static_cast<QFile*>(dev);

    // seek to the beginning
    file->reset();

    // Get writable buffer space for the chunk.
    WritableData writableData = getDataStorage( file->size() );
    if( writableData.isValid() ) {
        // Get pointer to start of writable memory.
        char* ptr = (char*) (writableData.ptr());
        file->read( ptr, file->size() );
    }

}
PELICAN_DECLARE(AbstractChunker, FileChunker)

} // namespace pelican
