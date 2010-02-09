#include "modules/ImagerFFT.h"
#include "utility/memCheck.h"
#include <iostream>

namespace pelican {

/**
 * @details
 * Module constructor.
 */
ImagerFFT::ImagerFFT(const QDomElement& config)
    : AbstractModule(config)
{
    // Register which data blobs are needed by the module
    addRemoteStreamData("VisibilityData");
    addLocalStreamData("VisibilityPositions");
    addLocalStreamData("ImageData");

    // Extract configuration from the xml configuration node.
    _getConfiguration(config);
}


/**
 * @details
 * Module destructor.
 */
ImagerFFT::~ImagerFFT()
{
}


/**
 * @details
 * Method called by the pipeline to create images of the visibility data.
 */
void ImagerFFT::run(QHash<QString, DataBlob*>& data)
{
    // Grab local pointers to the relevant data blobs.
    _vis = static_cast<VisibilityData*>(data["VisibilityData"]);
    _visCoord = static_cast<VisibilityPositions*>(data["VisiblityPositions"]);
    _image = static_cast<ImageData*>(data["ImageData"]);

    /// Check the data is available
    if (!_vis) throw QString("No visibility data,");
    if (!_visCoord) throw QString("No visibility coordinate data");
    if (!_image) throw QString("No image data");
}


/**
 * @details
 * Parse and extract options from the configuration xml node.
 * Default options are set if configuration options are not found.
 *
 * Defaults are:
 * - Cmage size: 128 x 128
 * - Cellsize:   10.0 x 10.0 [degrees]
 *
 * @param[in]   config  Configuraiton options xml node
 */
void ImagerFFT::_getConfiguration(const QDomElement &config)
{
    _sizeL = getOption("imageSize", "l", "128").toUInt();
    _sizeM = getOption("imageSize", "m", "128").toUInt();
    _cellsizeL = getOption("cellsize", "l", "10.0").toDouble();
    _cellsizeM = getOption("cellsize", "m", "10.0").toDouble();
}

} // namespace pelican
