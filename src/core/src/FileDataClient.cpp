#include "core/FileDataClient.h"
#include "data/DataRequirements.h"
#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * This creates a new abstract data client.
 */
FileDataClient::FileDataClient()
{
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
 * Read data blobs from a file data source and construct a hash that points to
 * available data.
 */
QHash<QString, DataBlob*> FileDataClient::getData(const DataRequirements& requiredData)
{

    return QHash<QString, DataBlob*>;
}

} // namespace pelican
