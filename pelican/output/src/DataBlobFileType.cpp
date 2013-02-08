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

#include "DataBlobFileType.h"
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QDataStream>

#include "data/DataBlob.h"

namespace pelican {

/**
 * @details Constructs a DataBlobFileType object.
 */
DataBlobFileType::DataBlobFileType(QFile* file, QSysInfo::Endian endian)
        : _file(file), _endian(endian)
{
    _readType();
}

/**
 * @details Destroys the DataBlobFileType object.
 */
DataBlobFileType::~DataBlobFileType()
{
    delete _file;
}

DataBlobFileHomogeneous::DataBlobFileHomogeneous(QFile* file, QSysInfo::Endian endian)
   : DataBlobFileType(file, endian)
{
}

DataBlobFileHeterogeneous::DataBlobFileHeterogeneous(QFile* file, QSysInfo::Endian endian)
   : DataBlobFileType(file, endian)
{
}

QString DataBlobFileType::nextBlob() const {
    return _blobType;
}

void DataBlobFileType::readData(DataBlob* blob) {
    Q_ASSERT( blob->type() == _blobType );
    blob->deserialise(*_file, _endian );
    if( _file->atEnd() ) {
        _blobType = "";
    }
}

void DataBlobFileHeterogeneous::readData(DataBlob* blob) {
     DataBlobFileType::readData(blob);
     _readType();
}

void DataBlobFileType::writeType(const QString& type, QIODevice* device)
{
    QDataStream out(device);
    out.setVersion(QDataStream::Qt_4_0);
    out << type;
}

void DataBlobFileType::_readType()
{
     if( _file->atEnd() ) {
        _blobType = "";
     }
     else {
         QDataStream in(_file);
         in.setVersion(QDataStream::Qt_4_0);
         in >> _blobType;
     }
}

DataBlobFileType* DataBlobFileType::openExistingFile(const QString& filename,
                                                     const QIODevice::OpenModeFlag mode)
{
    DataBlobFileType* fileType = 0;
    QFile* file = new QFile(filename);
    if (file->open(mode)) {
        QDataStream in(file);
        in.setVersion(QDataStream::Qt_4_0);
        QString magic;
        in >> magic;
        if( magic != DATABLOBFILE_MAGIC ) {
            delete file;
            throw(QString("DataBlobFile: \"") + filename
                  + "\" is not a DataBlobFileFormat file (magic=" + magic + ")");
        }
        int version;
        in >> version;
        if( version != DATABLOBFILE_VERSION ) {
            delete file;
            throw(QString("DataBlobFile: \"") + filename
                  + QString("\" file format is version %1. Incompatible with version %2")
                    .arg(version).arg(DATABLOBFILE_VERSION) );
        }
        //QSysInfo::Endian endian;
        int endian;
        in >> endian;
        if( endian != QSysInfo::BigEndian && endian != QSysInfo::LittleEndian ) {
            delete file;
            throw(QString("DataBlobFile: \"") + filename
                  + QString("\" Corrupted file - expecting an endian indicator (got %1)").arg(endian) );
        }
        int type;
        in >> type;
        switch( type ) {
            case Homogeneous:
                fileType = new DataBlobFileHomogeneous(file, (QSysInfo::Endian)endian);
                break;
            case Heterogeneous:
                fileType = new DataBlobFileHeterogeneous(file, (QSysInfo::Endian)endian);
                break;
            default:
                throw( QString("DataBlobFile: unknown file type :") + filename );
        }
    }
    else {
        delete file;
        throw( QString("DataBlobFile: Cannot open file : ") + filename );
    }
    return fileType;
}

} // namespace pelican
