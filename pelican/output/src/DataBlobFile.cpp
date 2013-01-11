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

#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <iostream>

#include "DataBlobFile.h"
#include "DataBlobFileType.h"
#include "data/DataBlob.h"
#include "utility/ConfigNode.h"

namespace pelican {


/**
 * @details Constructs a DataBlobFile object.
 */
DataBlobFile::DataBlobFile(const ConfigNode& configNode)
    : AbstractOutputStream(configNode), _blobType("")
{
    // Get the filename from the configuration node, and open it for output.
    const QDomElement& node = configNode.getDomElement();
    QDomNodeList list = node.elementsByTagName("file");
    for (int i = 0; i < list.size(); ++i) {
        QDomElement element = list.at(i).toElement();
        if (element.hasAttribute("name")) {
            QString type;
            if (element.hasAttribute("type")) {
                type = element.attribute("type");
            }
            else { type = "homogeneous"; }
            if( type == "homogeneous" ) {
                addFile( element.attribute("name"), DataBlobFileType::Homogeneous );
            }
            else if( type == "heterogeneous" ) {
                addFile( element.attribute("name"), DataBlobFileType::Heterogeneous );
            }
            else {
                throw( QString("DataBlobFile: unknown file type specified for file %1:")
                        .arg(element.attribute("name")) + type );
            }
        }
    }
}

/**
 * @details Destroys the DataBlobFile object.
 */
DataBlobFile::~DataBlobFile()
{
    foreach (QIODevice* device, _devices) {
        delete device;
    }
}

// Adds a file to the output stream and opens it for writing.
void DataBlobFile::addFile(const QString& filename, const DataBlobFileType::DataBlobFileType_t& type )
{
    verbose(QString("Creating file %1").arg(filename));
    QFile* file = new QFile(filename);
    if (file->open(QIODevice::WriteOnly)) {
        QDataStream out(file);
        out.setVersion(QDataStream::Qt_4_0);
        _devices.append(file);
        _hetero.insert(file, ((type==DataBlobFileType::Heterogeneous)?true:false) );
        out << QString(DATABLOBFILE_MAGIC);
        out << DATABLOBFILE_VERSION;
        out << QSysInfo::ByteOrder;
        out << type;
    }
    else {
        std::cerr << "Cannot open file for writing: "
                  << filename.toStdString() << std::endl;
        delete file;
    }
}

// Sends the data blob to the output stream.
void DataBlobFile::sendStream(const QString& /*streamName*/,
        const DataBlob* blob)
{
    // Output the string to each file
    foreach (QIODevice* device, _devices) {
        const QString& type=blob->type();
        if( _hetero[device] ) {
            // mark the blob type for heterogeneous blob files
            DataBlobFileType::writeType( type, device);
        }
        else if( type !=  _blobType ) {
             if( _blobType == "" ) {
                 DataBlobFileType::writeType( type, device);
                 _blobType=type;
             }
             else {
                verbose( QString("Attempt to write a DataBlob of type " + type
                             + " to homogenous file of type " + _blobType ));
                continue;
             }
        }
        blob->serialise(*device);
    }
}


} // namespace pelican
