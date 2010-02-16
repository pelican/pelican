#include "modules/ImagerDft.h"
#include "utility/constants.h"
#include <iostream>

#include "utility/memCheck.h"

namespace pelican {


/**
 * @details
 * Module constructor.
 */
ImagerDft::ImagerDft(const ConfigNode& config)
    : AbstractModule(config)
{
    // Register which data blobs are needed by the module
    addStreamData("VisibilityData");
    addGeneratedData("VisibilityPositions");
    addGeneratedData("ImageData");

    // Extract configuration from the XML configuration node.
    _getConfiguration(config);

    // Generate image pixel coordinate vectors based on the image configuration.
    _coordL.resize(_sizeL);
    _coordM.resize(_sizeM);
    _calculateImageCoords(_cellsizeL, _sizeL, &_coordL[0]);
    _calculateImageCoords(_cellsizeM, _sizeL, &_coordM[0]);
}


/**
 * @details
 * Module destructor.
 */
ImagerDft::~ImagerDft()
{
}


/**
 * @details
 * Method called by the pipeline to create images of the visibility data.
 */
void ImagerDft::run(QHash<QString, DataBlob*>& data)
{
    // Grab local pointers to the relevant data blobs.
    _vis = static_cast<VisibilityData*>(data["VisibilityData"]);
    _visCoord = static_cast<VisibilityPositions*>(data["VisiblityPositions"]);
    _image = static_cast<ImageData*>(data["ImageData"]);

    /// Check the data is available
    if (!_vis) throw QString("No visibility data,");
    if (!_visCoord) throw QString("No visibility coordinate data");
    if (!_image) throw QString("No image data");

    unsigned nPolarisations = 2;
    unsigned channel = _channel;
    unsigned nAntennas = _visCoord->nAntennas();
    unsigned nVis = nAntennas * nAntennas;
    real_t* uCoords = _visCoord->uPtr();
    real_t* vCoords = _visCoord->uPtr();
    real_t* lCoords = &_coordL[0];
    real_t* mCoords = &_coordM[0];
    _weightsUL.resize(nVis * _sizeL);
    _weightsVM.resize(nVis * _sizeM);
    complex_t* weightsUL = &_weightsUL[0];
    complex_t* weightsVM = &_weightsVM[0];
    double freqScale = _visCoord->freqScaleFactor(channel);

    _calculateWeights(uCoords, nVis, freqScale, lCoords, _sizeL, weightsUL);
    _calculateWeights(vCoords, nVis, freqScale, mCoords, _sizeM, weightsVM);

    for (unsigned p = 0; p < nPolarisations; p++) {
        real_t* image = _image->ptr(p, channel);
        complex_t* vis = _vis->ptr(channel, p);
        _makeImageDftSimple(nVis, _sizeL, _sizeM, vis, weightsUL, weightsVM, image);
        _cutHemisphere();
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
void ImagerDft::_getConfiguration(const ConfigNode &config)
{
    _sizeL = config.getOption("imageSize", "l", "128").toUInt();
    _sizeM = config.getOption("imageSize", "m", "128").toUInt();
    _cellsizeL = config.getOption("cellsize", "l", "10.0").toDouble();
    _cellsizeM = config.getOption("cellsize", "m", "10.0").toDouble();
    _channel = config.getOption("channel", "value", "0").toUInt();
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
void ImagerDft::_calculateImageCoords(const double cellsize,
        const unsigned nPixels, real_t* coords)
{
    double delta = cellsize * math::asec2rad;
    unsigned centre = nPixels / 2;
    for (unsigned i = 0; i < nPixels; i++) {
        coords[i] = static_cast<double>(i - centre) * delta;
    }
}



/**
 * @details
 * Calculates a matrix of complex weights for forming an image by
 * 2D DFT.
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
 * @param[in]  imageCoord   Image coordinate array (either l or m)
 * @param[in]  nCoords      Number of image coordinates
 * @param[in]  visCoord     Visibility coordinates array (either u or v)
 * @param[in]  nVis         Number of visibility coordinates
 * @param[in]  freqScale    Visibility coordinate frequency scaling factor
 * @param[out] weights      Matrix of DFT weights (nVis * nCoords)
 */
void ImagerDft::_calculateWeights(real_t* visCoord, const unsigned nVis,
        const double freqScale, real_t* imageCoord, const unsigned nCoords,
        complex_t* weights)
{
    for (unsigned j = 0; j < nCoords; j++) {
        unsigned index = j * nVis;
        double a = math::twoPi * imageCoord[j] * freqScale;
        for (unsigned i = 0; i < nVis; i++) {
            double arg = a * visCoord[i];
            weights[index + i] = complex_t(cos(arg), sin(arg));
        }
    }
}


/**
 * @details
 * Perform a discrete Fourier transform to form an image from the visibility data.
 *
 * @param[in]
 */
void ImagerDft::_makeImageDftSimple(const unsigned nVis, const unsigned nL,
        const unsigned nM, complex_t* vis, complex_t* weightUL,
        complex_t* weightVM, real_t *image)
{
#pragma omp parallel for
    for (unsigned m = 0; m < nM; m++) {
        unsigned indexM = m * nL;
        for (unsigned l = 0; l < nL; l++) {
            complex_t temp = complex_t(0.0, 0.0);
            for (unsigned i = 0; i < nVis; i++) {
                temp += vis[i] * weightUL[l * nVis + i] * weightVM[m * nVis + i];
            }
            unsigned index = indexM + l;
            image[index] = temp.real() / nVis;
        }
    }
}


/**
 * @details
 */
void ImagerDft::_cutHemisphere()
{
}


} // namespace pelican
