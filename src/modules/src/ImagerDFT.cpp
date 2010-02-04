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
    // Extract configuration from the xml configuration node.
    _getConfiguration(config);

    // Generate image pixel coordinate vectors based on the image configuration.
    _calculateImageCoords(_cellsizeL, _sizeL, _coordL);
    _calculateImageCoords(_cellsizeM, _sizeL, _coordM);
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
 */
void ImagerDFT::run(QHash<QString, DataBlob*>& data)
{
    // Grab local pointers to the relevant data blobs.
//    _vis = data["Visibilities"];
//    _visCoord = data["VisCoords"];
//    _image = data["Image"];

    /// loop over channels to generate images
    unsigned nChannels = 1; // all there?

    for (unsigned c = 0; c < nChannels; c++) {
        _calculateImageWeights(c, _visCoord->u(), _coordL, _weightUL);
        _calculateImageWeights(c, _visCoord->v(), _coordM, _weightVM);
        _makeImageDft(c);
        _cutHemisphere(); // optional??
    }
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
    if (_sizeL != getOption("imageSize", "l").toUInt()) {
        _sizeL = 128;
    }
    if (_sizeM != getOption("imageSize", "m").toUInt()) {
        _sizeL = 128;
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
 * Generate an image coordinate vector (\p coords) based on the pixel
 * separation in arcsec (\p cellsize) and number of pixels (\p nPixels).
 *
 * Image coordinates are for either the l (x) or m (y) depending on which
 * values are passed as input arguments.
 *
 * The pixel vector is resized to the number of pixels by the function.
 *
 * @param[in]  cellsize  Image pixel separation in arcseconds.
 * @param[in]  nPixels   Number of image pixels along the axis.
 * @param[out] coords    Reference to a vector of image coordinates.
 */
void ImagerDFT::_calculateImageCoords(
        const double cellsize,
        const unsigned nPixels,
        std::vector<real_t>& coords
){
    coords.resize(nPixels);
    double delta = cellsize * M_PI / 648000.0;
    unsigned centre = nPixels / 2;

    for (unsigned i = 0; i < nPixels; i++) {
        coords[i] = static_cast<double>(i - centre) * delta;
    }
}



/**
 * @details
 * Calculates a matrix of complex weights for forming an image by
 * 2D DFT at a specified frequency channel \p c for a set of visibility
 * coordinates (either u or v) and a set of image coordinates (either l or m).
 *
 * Weights take the form:
 * \f[
 *      W(l|m,u|v) = e^{2 \pi i \left ( ul |  vm) \right )}
 * \f]
 *
 * where \f$ u \f$ and \f$ v \f$ are visibility coordinates and depend on the
 * antenna positions and frequency channel and \f$ l \f$ and \f$ m \f$ are pixel
 * positions in radians.
 *
 * @param[in]  channel       Frequency channel for which to generate weights.
 * @param[in]  visCoord
 * @param[in]  imageCoord
 * @param[out] weight
 */
void ImagerDFT::_calculateImageWeights(
        unsigned channel,
        std::vector<real_t>& visCoord,
        std::vector<real_t>& imageCoord,
        std::vector<complex_t>& weight
){
    const double twoPi = 2.0 * M_PI;
    double freqScale = _visCoord->freqScaleFactor(channel);
    unsigned nVis = visCoord.size();
    unsigned nPixels = imageCoord.size();
    unsigned nWeights = nVis * nPixels;
    weight.resize(nWeights);

    // change loop order??
    for (unsigned j = 0; j < nVis; j++) {
        unsigned index = j * nPixels;
        double twoPiVis = twoPi * visCoord[j] * freqScale;
        for (unsigned i = 0; i < nPixels; i++) {
            double arg = twoPiVis * imageCoord[i];
            weight[index + i] = complex_t(cos(arg), sin(arg));
        }
    }
}


/**
 * @details
 * Perform a discrete Fourier transform to form an image from the visibility data.
 */
void ImagerDFT::_makeImageDft(unsigned channel)
{
    if (!_vis->nEntries()) {
        throw QString("Empty visibility matrix.");
    }
}




void ImagerDFT::_makeImageDft1(unsigned channel)
{
    const double twoPi = 2.0 * M_PI;
    double freqScale = _visCoord->freqScaleFactor(channel);
    unsigned nVis = visCoord.size();
    real_t *image = _image->ampPtr(polaristaion, channel);
    complex_t *visAmp = _vis->ptr(polarisation, channel);

    for (unsigned im = 0; im < _sizeM; im++) {
        unsigned index = im * _sizeL;
        for (unsigned il = 0; il < _sizeL; il++) {
            index += il;
            image[index] = 0.0;
            for (unsigned iv = 0; iv < nVis; iv++) {
//                image[index] +=
            }
        }
    }

}




/**
 * @details
 */
void ImagerDFT::_cutHemisphere()
{
}


} // namespace pelican
