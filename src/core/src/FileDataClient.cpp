#include "core/FileDataClient.h"
#include "data/DataRequirements.h"
#include "data/VisibilityData.h"
#include "data/FlagTable.h"

#include <QFile>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * This creates a new file data client.
 */
FileDataClient::FileDataClient(const QDomElement& config)
: AbstractDataClient(config)
{
    _fileVisibilities = getOption("fileVisibilities", "value");
    _nAntennas = getOption("nAntennas", "value", "0").toUInt();
    _nChannels = getOption("nChannels", "value", "0").toUInt();
    _nPolarisations = getOption("nPolarisations", "value", "0").toUInt();
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
 */
QHash<QString, DataBlob*> FileDataClient::getData(const DataRequirements& requiredData)
{
    /* Check if visibility data is required */
    if (requiredData.streamData().contains("VisibilityData"))
        _getVisibilityData(_fileVisibilities, _nAntennas, _nChannels, _nPolarisations);

    /* Check if flag table data is required */
    if (requiredData.serviceData().contains("FlagTable"))
        ensureFlagTableExists(_nAntennas, _nChannels, _nPolarisations);

    /* Return the data hash */
    return _dataHash;
}

/**
 * @details
 * This private method loads the visibility data from the given filename
 * into the VisibilityData data blob. Any existing visibility data is first
 * cleared from the data hash before the new visibilities are inserted.
 *
 * @param[in] filename  The filename to open.
 * @param[in] nAntennas The number of antennas in the visibility data.
 * @param[in] nChannels The number of channels in the visibility data.
 * @param[in] nPols     The number of polarisations in the visibility data.
 */
void FileDataClient::_getVisibilityData (
        QString filename,
        unsigned nAntennas,
        unsigned nChannels,
        unsigned nPols
){
    /* Check for and delete any existing visibility data */
    VisibilityData *visibilityData = static_cast<VisibilityData*>(_dataHash.value("VisibilityData"));
    if (visibilityData != NULL) {
        delete visibilityData;
        _dataHash.remove("VisibilityData");
    }

    /* Open the visibilities file */
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;

    /* Create a new visibility data blob and insert it into the hash */
    visibilityData = new VisibilityData(nAntennas, nChannels, nPols);
    _dataHash.insert("VisibilityData", visibilityData);
}

} // namespace pelican
