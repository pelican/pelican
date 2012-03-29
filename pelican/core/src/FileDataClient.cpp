#include "pelican/core/FileDataClient.h"
#include "pelican/core/AbstractServiceAdapter.h"
#include "pelican/core/AbstractStreamAdapter.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/utility/ConfigNode.h"

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
    foreach (DataRequirements req, dataRequirements()) {
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
