#include "core/AbstractDataClient.h"
#include "data/DataRequirements.h"
#include "data/DataBlob.h"
#include <QtGlobal>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * This creates a new abstract data client.
 */
AbstractDataClient::AbstractDataClient(const ConfigNode& config,
        DataBlobFactory* blobFactory)
{
    _config = &config;
    _blobFactory = blobFactory;
}

/**
 * @details
 * Destroys the data client.
 * This deletes all the data blobs still in the hash.
 */
AbstractDataClient::~AbstractDataClient()
{
    foreach (DataBlob* dataBlob, _dataHash) {
        delete dataBlob;
    }
    _dataHash.clear();
}

} // namespace pelican
