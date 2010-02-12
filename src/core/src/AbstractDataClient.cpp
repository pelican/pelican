#include "core/AbstractDataClient.h"
#include "data/DataBlob.h"
#include "data/FlagTable.h"
#include "data/DataRequirements.h"
#include "utility/memCheck.h"
#include <QtGlobal>

namespace pelican {

/**
 * @details
 * This creates a new abstract data client.
 */
AbstractDataClient::AbstractDataClient(const QDomElement& config)
{
    _config = config;
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

/**
 * @details
 * This protected method ensures that a clean flag table exists in the data hash.
 *
 * @param[in] nAntennas The number of antennas in the visibility data.
 * @param[in] nChannels The number of channels in the visibility data.
 * @param[in] nPols     The number of polarisations in the visibility data.
 */
void AbstractDataClient::ensureFlagTableExists (
        unsigned nAntennas,
        unsigned nChannels,
        unsigned nPols
){
    /* Check for an existing flag table and create a new one if it isn't present */
    FlagTable *flagTable = static_cast<FlagTable*>(_dataHash.value("FlagTable"));
    if (flagTable == NULL) {
        flagTable = new FlagTable(nAntennas, nChannels, nPols);
        _dataHash.insert("FlagTable", flagTable);
    } else {
        if (flagTable->nEntries() != nAntennas * nAntennas * nChannels * nPols)
            flagTable->resize(nAntennas, nChannels, nPols);
        flagTable->clear();
    }
}

} // namespace pelican
