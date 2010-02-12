#include "core/FileDataClient.h"
#include "data/DataRequirements.h"
#include "data/DataBlobFactory.h"

#include <QFile>
#include <QSet>
#include <QtGlobal>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * This creates a new file data client.
 */
FileDataClient::FileDataClient(const QDomElement& config,
        DataBlobFactory* blobFactory)
: AbstractDataClient(config, blobFactory)
{
    _fileVisibilities = getOption("fileVisibilities", "value");
}

/**
 * @details
 * Destroys the data client.
 * The base class destructor deletes all the data blobs still in the hash:
 * do not delete them here.
 */
FileDataClient::~FileDataClient()
{
}

/**
 * @details
 * Read data blobs from a file data source and fill the hash that points to
 * available data.
 *
 * @param[in] req All the data required by the pipeline application.
 *
 * @return The hash of data blobs.
 */
QHash<QString, DataBlob*>& FileDataClient::getData(const DataRequirements& req)
{
    /* Create a union of all data requirements */
    QSet<QString> allData = req.serviceData() & req.streamData();

    /* Iterate over the data requirements to create blobs if they don't exist */
    foreach (QString type, allData) {
        if (  ! _dataHash.contains(type) ) {
            _dataHash.insert(type, blobFactory()->create(type));
//            // open a file
//            QFile file(_filenames[type]);
//            QData
        }
    }

    /* Return the data hash */
    return _dataHash;
}

} // namespace pelican
