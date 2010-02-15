#include "core/FileDataClient.h"
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
FileDataClient::FileDataClient(const ConfigNode& config)
: AbstractDataClient(config)
{
    _fileVisibilities = config.getOption("fileVisibilities", "value");
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
 */
void FileDataClient::getData(const DataRequirements& req, QHash<QString, DataBlob*>& dataHash)
{
}

} // namespace pelican
