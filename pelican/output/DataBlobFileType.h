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

#ifndef DATABLOBFILETYPE_H
#define DATABLOBFILETYPE_H
#include <QtCore/QString>
#include <QtCore/QIODevice>
#include <QtCore/QSysInfo>
class QFile;

// type and version identifier for file header
#define DATABLOBFILE_MAGIC "DBlob"
#define DATABLOBFILE_VERSION 1
/**
 * @file DataBlobFileType.h
 */

namespace pelican {
class DataBlob;

/**
 * @class DataBlobFileType
 *
 * @brief
 *    Common data types for the DataBlobFile and DataBlobFileReader
 * @details
 *
 */
class DataBlobFileType
{
    public:
        typedef enum { Homogeneous, Heterogeneous } DataBlobFileType_t ;

    public:
        /// DataBlobFileType constructor.
        DataBlobFileType(QFile* file, QSysInfo::Endian endian);

        /// DataBlobFileType destructor.
        virtual ~DataBlobFileType();

        /// return the next datablob
        virtual QString nextBlob() const;

        /// read in data from the Blob
        virtual void readData(DataBlob* blob);

        static DataBlobFileType* openExistingFile(const QString& filename, QIODevice::OpenModeFlag);
        static void writeType(const QString& type, QIODevice* device);

    protected:
        void _readType();

    protected:
        QString _blobType;
        QFile* _file;
        QSysInfo::Endian _endian;

    private:
};

class DataBlobFileHeterogeneous : public DataBlobFileType {
    public:
        DataBlobFileHeterogeneous(QFile*, QSysInfo::Endian endian);
        void readData(DataBlob* blob);
};

class DataBlobFileHomogeneous: public DataBlobFileType {
    public:
        DataBlobFileHomogeneous(QFile*, QSysInfo::Endian endian);
};

} // namespace pelican

#endif // DATABLOBFILETYPE_H
