#include "modules/ImagerDFT.h"
#include "utility/memCheck.h"
#include <iostream>

namespace pelican {

/**
 * @details
 * Module constructor.
 */
ImagerDFT::ImagerDFT(const QDomElement& config)
    : AbstractModule(config)
{
    _getConfiguration(config);
}


/**
 * @details
 * Module destructor.
 */
ImagerDFT::~ImagerDFT()
{
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
void ImagerDFT::_getConfiguration(const QDomElement &config)
{
    if (_sizeL != getOption("imageSize", "l").toInt()) {
        _sizeL = 128;
    }
    if (_sizeM != getOption("imageSize", "m").toInt()) {
        _sizeM = 128;
    }
    if (_cellsizeL != getOption("cellsize", "l").toDouble()) {
        _cellsizeL = 10.0;
    }
    if (_cellsizeM != getOption("cellsize", "m").toDouble()) {
        _cellsizeM = 10.0;
    }
}


/**
 * @details
 * Perform a discrete Fourier transform to form an image from the visibility data.
 */
void ImagerDFT::_dft()
{
}


} // namespace pelican
