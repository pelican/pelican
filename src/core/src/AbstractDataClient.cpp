#include "core/AbstractDataClient.h"
#include "data/DataRequirements.h"
#include "data/DataBlob.h"
#include "utility/memCheck.h"
#include <QtGlobal>

namespace pelican {

/**
 * @details
 * This creates a new abstract data client.
 */
AbstractDataClient::AbstractDataClient(const QDomElement& config,
        DataBlobFactory* blobFactory)
{
    _config = config;
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

/**
 * @details
 * Gets the configuration for the given \p tagName and \p attribute.
 * If the \p tagName and/or \p attribute do not exist, \p defValue is returned.
 */
QString AbstractDataClient::getOption(const QString& tagName,
        const QString& attribute, const QString& defValue)
{
    return _config.namedItem(tagName).toElement().attribute(attribute,
            defValue);
}

} // namespace pelican
