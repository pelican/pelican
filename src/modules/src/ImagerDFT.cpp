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
    // Register which data blobs are needed by the module
    addRemoteStreamData("VisibilityData");
    addLocalStreamData("VisibilityPositionsData");
    addLocalStreamData("ImageData");

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
 * Method called by the pipeline to create images of the visibility data.
 */
void ImagerDFT::run(QHash<QString, DataBlob*>& data)
{
    // Grab local pointers to the relevant data blobs.
    _vis = static_cast<VisibilityData*>(data["VisibilityData"]);
    _visCoord = static_cast<VisibilityPositionsData*>(data["VisiblityPositionsData"]);
    _image = static_cast<ImageData*>(data["ImageData"]);

    /// Check the data is available
    if (!_vis) throw QString("No visibility data,");
    if (!_visCoord) throw QString("No visibility coordinate data");
    if (!_image) throw QString("No image data");

    /// Loop over polarisations and channels to generate images.
    // need to find if all are left after flagging...
    unsigned nChannels = 1;
    unsigned nPolarisations = 1;
    for (unsigned c = 0; c < nChannels; c++) {
        _calculateImageWeights(c, _visCoord->u(), _coordL, _weightUL);
        _calculateImageWeights(c, _visCoord->v(), _coordM, _weightVM);
        for (unsigned p = 0; p < nPolarisations; p++) {
//            _makeImageDft(p, c, _weightUL, _weightVM, _image->ptr(p, c));
//            _cutHemisphere();
        }
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
    _sizeL = getOption("imageSize", "l", "128").toUInt();
    _sizeM = getOption("imageSize", "m", "128").toUInt();
    _cellsizeL = getOption("cellsize", "l", "10.0").toDouble();
    _cellsizeM = getOption("cellsize", "m", "10.0").toDouble();
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
 * @param[in]  c            Frequency channel for which to generate weights.
 * @param[in]  visCoord
 * @param[in]  imageCoord
 * @param[out] weight
 */
void ImagerDFT::_calculateImageWeights(
        unsigned c,
        std::vector<real_t>& visCoord,
        std::vector<real_t>& imageCoord,
        std::vector<complex_t>& weight
){
//    const double twoPi = 2.0 * M_PI;
//    double freqScale = _visCoord->freqScaleFactor(c);
//    unsigned nVis = visCoord.size();
//    unsigned nCoords = imageCoord.size();
//    unsigned nWeights = nVis * nPixels;
//    weight.resize(nWeights);
//
//    for (unsigned j = 0; j < nCoords; i++) {
//        unsigned index = j * nVis;
//
//        for (unsigned i = 0; i < nVis; i++) {
//            double twoPiVis = twoPi * visCoord[i] * freqScale;
//            double arg = twoPiVis * imageCoord[j];
//            weight[index + i] = complex_t(cos(arg), sin(arg));
//        }
//    }
}


/**
 * @details
 * Perform a discrete Fourier transform to form an image from the visibility data.
 */
void ImagerDFT::_makeImageDft(unsigned p, unsigned c)
{
    if (!_vis->nEntries()) {
        throw QString("Empty visibility matrix.");
    }
}


/**
 * @details
 */
void ImagerDFT::_makeImageDft1(
        unsigned p, unsigned c,
        std::vector<complex_t>& visAmp,
        std::vector<complex_t>& weightUL,
        std::vector<complex_t>& weightVM,
        real_t* image
){
//    unsigned nAnt = 0;
//
//    for (unsigned im = 0; im < _sizeM; im++) {
//        unsigned index = im * _sizeL;
//        for (unsigned il = 0; il < _sizeL; il++) {
//            index += il;
//            image[index] = 0.0;
//            for (unsigned j; j < nAnt; j++) {
//                for (unsigned i; i < nAnt; i++) {
////                    image[index] += visAmp(i, j) * weightUL[i] * weightVM[j];
//                }
//            }
//        }
//    }
}


/**
 * @details
 */
void ImagerDFT::_cutHemisphere()
{
}


} // namespace pelican
