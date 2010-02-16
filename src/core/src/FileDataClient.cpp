#include "core/FileDataClient.h"
#include "adapters/AbstractServiceAdapter.h"
#include "adapters/AbstractStreamAdapter.h"
#include "adapters/AdapterFactory.h"
#include "data/DataRequirements.h"
#include "data/DataBlobFactory.h"
#include "utility/ConfigNode.h"

#include <QFile>
#include <QSet>
#include <QtGlobal>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * This creates a new file data client.
 */
FileDataClient::FileDataClient(const ConfigNode& config,
        AdapterFactory* adapterFactory,
        QList<DataRequirements>& dataRequirements)
: AbstractDataClient(config, adapterFactory, dataRequirements)
{
    /* Get the configuration options */
    _getConfig();

    /* Loop over data requirements for each pipeline */
    foreach (DataRequirements req, dataRequirements) {
        /* Create a union of required data types for this pipeline */
        QSet<QString> allDataReq = req.serviceData() & req.streamData();

        /* Loop over each data type to set up the adapters for each pipeline */
        adapters().append(QHash<QString, AbstractAdapter*>());
        foreach (QString type, allDataReq) {
            AbstractAdapter* adapter = adapterFactory->create(adapterNames().value(type));
            adapters().last().insert(type, adapter);
        }
    }
}

/**
 * @details
 * Destroys the data client.
 */
FileDataClient::~FileDataClient()
{
}

/**
 * @details
 * Read data blobs from a file data source and fill the hash that points to
 * available data.
 */
QHash<QString, DataBlob*> FileDataClient::getData(QHash<QString, DataBlob*>& dataHash)
{
    /* Create the local data hash to return */
    QHash<QString, DataBlob*> validHash;

    /* Loop over each pipeline's set of data requirements */
    unsigned pipelineIndex = 0;
    foreach (DataRequirements req, dataRequirements()) {

        /* Loop over service data requirements */
        foreach (QString type, req.serviceData()) {
            QString filename = _fileNames.value(type);
            if (!filename.isEmpty()) {
                QFile file(filename);
                if (!file.open(QIODevice::ReadOnly))
                    throw QString("Cannot open file.");
                QDataStream in(&file);
                AbstractAdapter* ad = adapters().at(pipelineIndex).value(type);
                AbstractServiceAdapter* adapter = static_cast<AbstractServiceAdapter*>(ad);
                in >> adapter->config(dataHash.value(type), 0);
            }
        }

        /* Loop over stream data requirements */
        foreach (QString type, req.streamData()) {
            QString filename = _fileNames.value(type);
            if (!filename.isEmpty()) {
                QFile file(filename);
                if (!file.open(QIODevice::ReadOnly))
                    throw QString("Cannot open file.");
                QDataStream in(&file);
                AbstractAdapter* ad = adapters().at(pipelineIndex).value(type);
                AbstractStreamAdapter* adapter = static_cast<AbstractStreamAdapter*>(ad);
                QHash<QString, DataBlob*> hash;
                in >> adapter->config(dataHash.value(type), 0, hash);
            }
        }
        pipelineIndex++;
    }

    return validHash;
}

/**
 * @details
 * Gets the configuration options from the XML configuration node.
 */
void FileDataClient::_getConfig()
{
    /* Get all the filenames for each data type */
    _fileNames = configNode()->getOptionHash("data", "type", "name");
}

} // namespace pelican
