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
    if (_sizex != getOption("imageSize", "x").toInt()) {
        _sizex = 128;
    }
    if (_sizey != getOption("imageSize", "y").toInt()) {
        _sizey = 128;
    }
    if (_cellsizex != getOption("cellsize", "x").toDouble()) {
        _cellsizex = 10.0;
    }
    if (_cellsizex != getOption("cellsize", "x").toDouble()) {
        _cellsizex = 10.0;
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
