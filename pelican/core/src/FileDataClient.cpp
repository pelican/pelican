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

#include "core/FileDataClient.h"
#include "core/AbstractServiceAdapter.h"
#include "core/AbstractStreamAdapter.h"
#include "data/DataRequirements.h"
#include "utility/ConfigNode.h"

#include <QtCore/QFile>
#include <QtCore/QSet>
#include <QtCore/QtGlobal>

namespace pelican {

/**
 * @details
 * This creates a new file data client.
 */
FileDataClient::FileDataClient(const ConfigNode& configNode,
        const DataTypes& types, const Config* config)
        : AbstractAdaptingDataClient(configNode, types, config)
{
    // Get the configuration options.
    _getConfig();
}

/**
 * @details
 * Destroys the data client.
 */
FileDataClient::~FileDataClient()
{
     foreach(QFile* file, _openFiles) {
        delete file;
     }
}


/**
 * @details
 * Read data blobs from a file data source and fill the hash that points to
 * available data.
 */
AbstractDataClient::DataBlobHash FileDataClient::getData(DataBlobHash& dataHash)
{
    // Create the local data hash to return.
    DataBlobHash validHash;

    // Loop over each pipeline's set of data requirements.
    foreach(const DataSpec& req, dataRequirements()) {
        // Loop over service data requirements.
        foreach (QString type, req.serviceData())
        {
            if( ! dataHash.contains(type) )
                throw( QString("FileDataClient: getData() called without DataBlob %1").arg(type) );
            if( ! _openFiles.contains(type) || _openFiles[type]->atEnd() ) {
                if( ! _openFile(type) ) continue;
            }

            /* -------
            QString filename = _fileNames.value(type);
            if (!filename.isEmpty()) {
                QFile file(filename);
                if (!file.open(QIODevice::ReadOnly))
                    throw QString("FileDataClient::getData(): "
                            "Cannot open file %1").arg(filename);
             */
            QFile* file = _openFiles[type];
            AbstractServiceAdapter* adapter = serviceAdapter(type);
            Q_ASSERT( adapter != 0 );
            adapter->config(dataHash[type], file->size());
            adapter->deserialise(file);
            validHash.insert(type, dataHash.value(type));
        }

        // Loop over stream data requirements.
        foreach (QString type, req.streamData())
        {
            if( ! dataHash.contains(type) )
                throw( QString("FileDataClient: getData() called without DataBlob %1").arg(type) );
            if( ! _openFiles.contains(type) || _openFiles[type]->atEnd() ) {
                if( ! _openFile(type) ) continue;
            }
            /* -------
            QString filename = _fileNames.value(type);
            if (!filename.isEmpty()) {
                QFile file(filename);
                if (!file.open(QIODevice::ReadOnly))
                    throw QString("FileDataClient::getData(): "
                            "Cannot open file %1").arg(filename);
             */
            QFile* file = _openFiles[type];
            AbstractStreamAdapter* adapter = streamAdapter(type);
            Q_ASSERT( adapter != 0 );
            QHash<QString, DataBlob*> serviceHash;
            adapter->config(dataHash[type], file->size(), serviceHash);
            adapter->deserialise(file);
            validHash.insert(type, dataHash.value(type));
        }
    }

    return validHash;
}

bool FileDataClient::_openFile( const QString& type )
{
    QString filename = _fileNames.value(type);
    if (!filename.isEmpty()) {
        QFile* file = new QFile(filename);
        log(QString("Opening file %1").arg(filename));
        if (!file->open(QIODevice::ReadOnly)) {
            delete file;
            throw QString("FileDataClient::getData(): "
                    "Cannot open file %1").arg(filename);
        }
        if( _openFiles.contains(type) )
            delete _openFiles[type];
        _openFiles.insert(type, file);
        return true;
    }
    return false;
}

const DataSpec& FileDataClient::dataSpec() const {
    return _dataSpec;
}

/**
 * @details
 * Gets the configuration options from the XML configuration node.
 */
void FileDataClient::_getConfig()
{
    // Get all the filenames for each data type.
    _fileNames = configNode().getOptionHash("data", "type", "file");
}

} // namespace pelican
