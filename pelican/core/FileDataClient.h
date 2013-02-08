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

#ifndef FILEDATACLIENT_H
#define FILEDATACLIENT_H

/**
 * @file FileDataClient.h
 */

#include "core/AbstractAdaptingDataClient.h"
#include "data/DataSpec.h"
#include <QtCore/QHash>
#include <QtCore/QString>
class QFile;

namespace pelican {

class DataBlob;
class ConfigNode;
class DataTypes;

/**
 * @ingroup c_core
 *
 * @class FileDataClient
 *
 * @brief
 * Data client used by the pipeline for accessing data directly from files
 * rather than using the data server.
 *
 * @details
 * This data client fetches data directly from one or more data files and
 * makes it available to the pipelines via the pipeline driver.
 */

class FileDataClient : public AbstractAdaptingDataClient
{
    public:
        /// Data client constructor.
        FileDataClient(const ConfigNode& configNode,
                const DataTypes& types, const Config* config);

        /// Data client destructor
        virtual ~FileDataClient();

    public:
        /// Gets the requested data.
        virtual DataBlobHash getData(DataBlobHash&);
        virtual const DataSpec& dataSpec() const;

    private:
        /// Reads the configuration options.
        void _getConfig();
        bool _openFile(const QString& type);

    private:
        // Hash of filenames for each data type.
        QHash<QString, QString> _fileNames;
        QHash<QString, QFile*> _openFiles;
        DataSpec _dataSpec;
};

PELICAN_DECLARE_CLIENT(FileDataClient)

} // namespace pelican
#endif // FILEDATACLIENT_H
