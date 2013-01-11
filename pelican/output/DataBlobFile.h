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

#ifndef DATABLOBFILE_H
#define DATABLOBFILE_H

#include <QtCore/QList>
#include <QtCore/QHash>
class QIODevice;
class QFile;

#include "DataBlobFileType.h"

#include "output/AbstractOutputStream.h"

/**
 * @file DataBlobFile.h
 */

namespace pelican {

/**
 * @class DataBlobFile
 *
 * @brief
 *    write datablobs to a file
 * @details
 *    opens one or more files on a device and streams any
 *    DataBlob that supports the serialise() method to the file
 *    The file nay be of homogenous or mixed DataBlob types
 *    (which incur a storage overhead).
 * @configuration
 * The default type of file is "homogenous". this must be set to heterogenous
 * if you intend to store different datablob types in the same file
 * <DataBlobFile>
 *     <file name="file1.output">
 *     <file name="duplicatefile.output" type="homogenous">
 *     <file name="hetroFormat.output" type="heterogenous">
 * <DataBlobFile>
 */
class DataBlobFile : public AbstractOutputStream
{

    public:
        /// DataBlobFile constructor.
        DataBlobFile(const ConfigNode& configNode);

        /// DataBlobFile destructor.
        virtual ~DataBlobFile();

        // Add a file for output to be saved.
        void addFile(const QString& filename, const DataBlobFileType::DataBlobFileType_t& type );

        // write the data
        virtual void sendStream(const QString& streamName,
                                const DataBlob* blob);


    private:
        QString _blobType;
        QList<QIODevice*> _devices;
        QHash<QIODevice*,bool> _hetero;
};

PELICAN_DECLARE(AbstractOutputStream, DataBlobFile )

} // namespace pelican

#endif // DATABLOBFILE_H
