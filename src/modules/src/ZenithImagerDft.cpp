#include "modules/ZenithImagerDft.h"
#include "utility/constants.h"
#include "utility/pelicanTimer.h"
#include "data/VisibilityData.h"
#include "data/AntennaPositions.h"
#include "data/ImageData.h"
#include <QString>
#include <QStringList>
#include <iostream>
#include <iomanip>
#include <limits>

#ifdef USE_CBLAS
    #ifdef USE_MKL
        #include <mkl_cblas.h>
    #else
        #include "cblas.h"
    #endif
#endif

#ifdef PELICAN_OPENMP
    #include <omp.h>
#endif

#include "utility/memCheck.h"


namespace pelican {

/**
 * @details
 * Module constructor.
 */
ZenithImagerDft::ZenithImagerDft(const ConfigNode& config)
    : AbstractModule(config)
{
    // Extract configuration from the xml configuration node.
    _getConfiguration(config);

    // Register which data blobs are needed by the module
    if (_useModelVis) {
        addGeneratedData("ModelVisibilityData");
    }
    else if (_useRawVis) {
        addStreamData("VisibilityData");
    }
    else {
        addGeneratedData("CorrectedVisibilityData");
    }
    addServiceData("AntennaPositions");
    addGeneratedData("ImageData");

    // Initialise local data pointers
    _vis = NULL;
    _antPos = NULL;
    _image = NULL;

    // Generate image pixel coordinate vectors based on the configuration.
    _coordL.resize(_sizeL);
    _coordM.resize(_sizeM);
    _calculateImageCoords(_cellsizeL, _sizeL, &_coordL[0]);
    _calculateImageCoords(_cellsizeM, _sizeM, &_coordM[0]);
}


/**
 * @details
 * Sets the image size to the specified values and calculates
 * image coordinate vectors.
 */
void ZenithImagerDft::setSize(const unsigned sizeL, const unsigned sizeM)
{
    _sizeL = sizeL;
    _sizeM = sizeM;
    _coordL.resize(_sizeL);
    _coordM.resize(_sizeM);
    if (_fullSky) _setCellsizeFullSky();
    _calculateImageCoords(_cellsizeL, _sizeL, &_coordL[0]);
    _calculateImageCoords(_cellsizeM, _sizeM, &_coordM[0]);
}


/**
 * @details
 * Sets the image cellsize to the specified values and calculates
 * image coordinate vectors.
 */
void ZenithImagerDft::setCellsize(const double cellsizeL,
        const double cellsizeM)
{
    _fullSky = false;
    _cellsizeL = -std::abs(cellsizeL);
    _cellsizeM = cellsizeM;
    _calculateImageCoords(_cellsizeL, _sizeL, &_coordL[0]);
    _calculateImageCoords(_cellsizeM, _sizeM, &_coordM[0]);
}


/**
 * @details
 * Sets the image size and cellsize to the specified values and
 * calculate image coordinate vectors.
 */
void ZenithImagerDft::setDimensions(const unsigned sizeL, const unsigned sizeM,
                const double cellsizeL, const double cellsizeM)
{
    _fullSky = false;
    _cellsizeL = -std::abs(cellsizeL);
    _cellsizeM = cellsizeM;
    _sizeL = sizeL;
    _sizeM = sizeM;
    _coordL.resize(_sizeL);
    _coordM.resize(_sizeM);
    _calculateImageCoords(_cellsizeL, _sizeL, &_coordL[0]);
    _calculateImageCoords(_cellsizeM, _sizeM, &_coordM[0]);
}


/**
 * @details
 * Sets the imager to make a full sky image.
 */
void ZenithImagerDft::setFullSky()
{
    _fullSky = true;
    _setCellsizeFullSky();
    _calculateImageCoords(_cellsizeL, _sizeL, &_coordL[0]);
    _calculateImageCoords(_cellsizeM, _sizeM, &_coordM[0]);
}


/**
 * @details
 * Method called by the pipeline to create images of the visibility data.
 */
void ZenithImagerDft::run(QHash<QString, DataBlob*>& data)
{
    // Get the data blobs.
    _fetchDataBlobs(data);

    unsigned nAnt = _antPos->nAntennas();
    unsigned nPolImage = _polarisation == POL_BOTH ? 2 : 1;
    unsigned nChanImage = _channels.size();


    // Assign memory for the image cube (only resizes if needed).
    _image->resize(_sizeL, _sizeM, nChanImage, nPolImage);

    // Set the image blob meta-data.
    _image->cellsizeL() = _cellsizeL;
    _image->cellsizeM() = _cellsizeM;
    _image->refCoordL() = 0; // Set the RA at the image centre.
    _image->refCoordM() = 89.99; // Set the Dec at the image centre.
    _image->refPixelL() = _sizeL / 2;
    _image->refPixelM() = _sizeM / 2;
    complex_t* vis = NULL;

    // Loop over selected channels and polarisations to make images.
    for (unsigned c = 0; c < nChanImage; c++) {

        // The channel ID selection
        unsigned channel = _channels[c];
        double frequency = _freqRef + (channel - _freqRefChannel) * _freqDelta;
//        std::cout << "frequency (imager)= " << frequency <<std::endl;

        for (unsigned p = 0; p < nPolImage; p++) {

            unsigned pol = p;
//            if (nPolImage == 1) pol = _polarisation; // old
            pol = (nPolImage == 1 && _polarisation == POL_X) ? 0 : 1;

            // Get pointers to the visibility data and image for the selected
            // channel and polarisation.
            if (_useModelVis) vis = _vis->ptr(c, p);
            else vis = _vis->ptr(channel, pol);

            if (_pointSpreadFunction) {
                _setPsfVisibilties(vis, nAnt);
            }
            else {
                _zeroAutoCorrelations(vis, nAnt);
            }

            real_t* image = _image->ptr(c, p);
            // Generate the image.
            _makeImageDft(nAnt, _antPos->xPtr(), _antPos->yPtr(), vis, frequency,
                    _sizeL, _sizeM, &_coordL[0], &_coordM[0], image);

            _image->calculateMean(c, p);

            // Cut hemisphere.
            if (_fullSky && _trimHemisphere) {
                _cutHemisphere(image, _sizeL, _sizeM, &_coordL[0], &_coordM[0]);
            }

            // Find the amplitude range.
            _image->calculateAmplitudeRange(c, p);

            if (isnan(_image->max(c, p)) || isnan(_image->min(c, p)))
                throw QString ("ZenithImagerDft: invalid Image range");
        }
    }
}


/**
 * @details
 * Parse and extract options from the configuration XML node.
 * Default options are set if configuration options are not found.
 *
 * @param[in]   config  Configuration options XML node.
 */
void ZenithImagerDft::_getConfiguration(const ConfigNode& config)
{
    _sizeL = config.getOption("size", "l", "128").toUInt();
    _sizeM = config.getOption("size", "m", "128").toUInt();
    _fullSky = config.getOption("fullSky", "value", "true") == "true" ? true : false;
    if (config.getOption("fullSky", "cutHemisphere", "true") == "true") {
        _trimHemisphere = true;
    }
    else {
        _trimHemisphere = false;
    }

    // Full sky = set l and m cellsize for a full image ignoring other settings.
    if (_fullSky) {
        _setCellsizeFullSky();
    }
    else {
        _cellsizeL = -std::abs(config.getOption("cellsize", "l", "10.0").toDouble());
        _cellsizeM = config.getOption("cellsize", "m", "10.0").toDouble();
    }

    // Get the polarisation selection.
    QString pol = config.getOption("polarisation", "value", "x").toLower();
    if (pol == "x") _polarisation = POL_X;
    else if (pol == "y") _polarisation = POL_Y;
    else if (pol == "both") _polarisation = POL_BOTH;

    // Get the channels to image.
    QString chan = config.getOptionText("channels", "0");
    QStringList chanList = chan.split(",", QString::SkipEmptyParts);
    _channels.resize(chanList.size());
    for (int c = 0; c < chanList.size(); c++) {
        _channels[c] = chanList.at(c).toUInt();
    }


    /* A/D-converter – to be able to cope with expected interference levels – operating at either
    160 or 200 MHz in the first, second or third Nyquist zone (i.e., 0 - 100, 100 - 200, or 200 -
    300MHz band respectively for 200MHz sampling). The data from the receptors is filtered
    in 512×195 kHz sub-bands (156 kHz subbands for 160MHz sampling) of which a total
    of 3MHz bandwidth (164 channels) can be used at any time */
    _freqRefChannel = config.getOption("frequencies", "referenceChannel","0").toInt();
    _freqRef = config.getOption("frequencies", "reference", "1.0e8").toDouble();
    _freqDelta = config.getOption("frequencies", "delta", "1.0e8").toDouble();

    _pointSpreadFunction = (config.getOption("pointSpreadFunction", "value", "false") == "true") ?
            true : false;

    _pixelCentred = (config.getOption("size", "pixelCentred", "false") == "true") ?
            true: false;

    _useModelVis = (config.getOption("useModelVis", "value", "false") == "true") ?
            true : false;

    _useRawVis = (config.getOption("useRawVis", "value", "false") == "true") ?
            true : false;
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
void ZenithImagerDft::_calculateImageCoords(const double cellsize,
        const unsigned nPixels, real_t* coords)
{
    if (coords == NULL)
        throw QString("ZenithImagerDft::_calculateImageCoords(): coordinate array not assigned");

    if (nPixels == 0)
        throw QString("ZenithImagerDft::_calculateImageCoords(): Zero image pixels!");

    if (nPixels == 1) {
        coords[0] = 0.0;
        return;
    }

    double delta = cellsize * math::asec2rad;
    int centre = nPixels / 2;
    double offset = (_pixelCentred) ? 0 : delta / 2.0;

    for (int i = 0; i < static_cast<int>(nPixels); i++) {
        coords[i] = static_cast<double>(i - centre) * delta + offset;
    }
}


/**
 * @details
 * Fetches named data blobs from the data blob hash into local local varibles.
 */
void ZenithImagerDft::_fetchDataBlobs(QHash<QString, DataBlob*>& data)
{
    if (_useModelVis) {
        _vis = static_cast<VisibilityData*>(data["ModelVisibilityData"]);
    }
    else if (_useRawVis) {
        _vis = static_cast<VisibilityData*>(data["VisibilityData"]);
    }
    else {
        _vis = static_cast<VisibilityData*>(data["CorrectedVisibilityData"]);
    }

    _antPos = static_cast<AntennaPositions*>(data["AntennaPositions"]);
    _image = static_cast<ImageData*>(data["ImageData"]);

    if (!_vis)
        throw QString("ZenithImagerDft: VisibilityData blob missing.");
    if (!_antPos)
        throw QString("ZenithImagerDft: AntennaPositiosn blob missing.");
    if (!_image)
        throw QString("ZenithImagerDft: ImageData blob missing.");

    if (_vis->nAntennas() == 0)
        throw QString("Empty data blob: VisibilityData.");
    if (_antPos->nAntennas() == 0)
        throw QString("Empty data blob: AntennaPositions.");

    if (_vis->nAntennas() != _antPos->nAntennas())
        throw QString("ZenithImagerDft: data blob dimension mismatch.");
}


/**
 * @details
 * Calculates a matrix of complex weights for forming an image by
 * 2D DFT.
 *
 * @param[in]   nCoords Number
 */
void ZenithImagerDft::_calculateWeights(const unsigned nAnt, real_t* antPos,
        const double frequency, const unsigned nCoords,
        real_t* imageCoord, complex_t* weights)
{
    double k = (math::twoPi * frequency) / phy::c;

    for (unsigned i = 0; i < nCoords; i++) {
        unsigned index = i * nAnt;
        double arg1 = k * imageCoord[i];

        for (unsigned a = 0; a < nAnt; a++) {
            double arg2 = arg1 * antPos[a];
            weights[index + a] = complex_t(cos(arg2), sin(arg2));
        }
    }
}


/**
 * @details
 * Perform a discrete Fourier transform to form an image from the visibility data.
 */
void ZenithImagerDft::_makeImageDft(const unsigned nAnt, real_t* antPosX,
        real_t* antPosY, complex_t* vis, const double frequency,
        const unsigned nL, const unsigned nM,
        real_t* coordsL, real_t* coordsM, real_t *image)
{
    _weightsXL.resize(nAnt * nL);
    _weightsYM.resize(nAnt * nM);

    _calculateWeights(nAnt, antPosX, frequency, nL, coordsL, &_weightsXL[0]);
    _calculateWeights(nAnt, antPosY, frequency, nM, coordsM, &_weightsYM[0]);

    // Set up buffers for sorting a vector of weights for one pixel
    // and the temporary results of the per pixel matrix vector product
#ifdef PELICAN_OPENMP
    unsigned nProcs = omp_get_num_procs();
    std::vector< std::vector<complex_t> > tempWeights;
    std::vector< std::vector<complex_t> > tempBuffer;
    tempWeights.resize(nProcs);
    tempBuffer.resize(nProcs);
    for (unsigned i = 0; i < nProcs; i++) {
        tempWeights[i].resize(nAnt);
        tempBuffer[i].resize(nAnt);
    }
    int tid = 0;
#else
    std::vector<complex_t> tempWeights(nAnt);
    std::vector<complex_t> tempBuffer(nAnt);
#endif

#ifdef USE_CBLAS
    real_t alpha[2] = {1.0, 0.0};
    real_t beta[2]  = {0.0, 0.0};
    unsigned xInc = 1;
    unsigned yInc = 1;
#endif

    complex_t* weights = NULL;
    complex_t* buffer = NULL;
    unsigned nNonZeroVis = nAnt * nAnt - nAnt;

    // Loop over image pixels to calculate the image amplitude using a
    // 2 sided matrix vector approach.
#pragma omp parallel for private(tid, weights, buffer)
    for (int m = 0; m < static_cast<int>(nM); m++) {

            unsigned indexM = m * nL;
            complex_t *weightsYM = &_weightsYM[m * nAnt];

#ifdef PELICAN_OPENMP
            tid = omp_get_thread_num();
            weights = &tempWeights[tid][0];
            buffer = &tempBuffer[tid][0];
#else
            weights = &tempWeights[0];
            buffer = &tempBuffer[0];
#endif

        for (unsigned l = 0; l < nL; l++) {
            complex_t* weightsXL = &_weightsXL[l * nAnt];
            unsigned index = indexM + l;

            _multWeights(nAnt, weightsXL, weightsYM, weights);

#ifdef USE_CBLAS
            // (y := alpha*A*x + beta*y)
            // where: y = temp
            //        A = vis
            //        x = weights
            cblas_zgemv(CblasRowMajor, CblasNoTrans, nAnt, nAnt, alpha, vis,
                    nAnt, weights, xInc, beta, buffer, yInc);
#else
            _multMatrixVector(nAnt, vis, weights, buffer);
#endif

            /// TODO: Use some sort of cblas_zdot to replace this call.
            image[index] = _vectorDotConj(nAnt, buffer, weights).real();

            // normalise - (as for each non zero vis/weight the sum gets an
            // extra factor of 1 from from the e^(i...))
            image[index] /= static_cast<double>(nNonZeroVis);
        }
    }
}


/**
 * @details
 * Element wise multiplication of two weights vectors.
 * - need to find a blas function to do this...
 */
void ZenithImagerDft::_multWeights(const unsigned nAnt, complex_t* weightsXL,
        complex_t *weightsYM, complex_t *weights)
{
    for (unsigned i = 0; i < nAnt; i++) {
        weights[i] = weightsXL[i] * weightsYM[i];
    }
}


/**
 * @details
 * Performs matrix vector multiply of a matrix of visibility amplitudes
 * by a vector of complex dft weights
 */
void ZenithImagerDft::_multMatrixVector(const unsigned nAnt,
        complex_t* visMatrix, complex_t *weights, complex_t* result)
{
    for (unsigned j = 0; j < nAnt; j++) {
        unsigned rowIndex = j * nAnt;
        result[j] = complex_t(0.0, 0.0);
        for (unsigned i = 0; i < nAnt; i++) {
            unsigned index = rowIndex + i;
            result[j] += visMatrix[index] * weights[i];
        }
    }
}


/**
 * @details
 * Vector dot product
 */
complex_t ZenithImagerDft::_vectorDotConj(const unsigned n, complex_t* a,
        complex_t* b)
{
    complex_t result = complex_t(0.0, 0.0);
    for (unsigned i = 0; i < n; i++) {
        result += a[i] * conj(b[i]);
    }
    return result;
}



/**
 * @details
 *
 * @param[in/out]   image   Image amplitude array.
 * @param[in]       nL      Number of image pixels in the l (x) direction.
 * @param[in]       nM      Number of image pixels in the l (y) direction.
 * @param[in]       l       Array of l coordinates.
 * @param[in]       m       Array of m coordinates.
 */
void ZenithImagerDft::_cutHemisphere(real_t* image, const unsigned nL,
        const unsigned nM, real_t *l, real_t *m)
{
    for (unsigned j = 0; j < nM; j++) {
        unsigned rowIndex = j * nL;
        double m2 = std::pow(m[j], 2.0);

        for (unsigned i = 0; i < nL; i++) {
            double l2 = std::pow(l[i], 2.0);
            double radius = sqrt(m2 + l2);
            unsigned index = rowIndex + i;
            if (radius > 1) {
                image[index] = std::numeric_limits<real_t>::quiet_NaN();
            }
        }
    }
}


/**
 * @details
 * Sets the cellsize required for full sky imaging.
 */
void ZenithImagerDft::_setCellsizeFullSky()
{
    _cellsizeL = -2.0 / _sizeL * math::rad2asec;
    _cellsizeM = 2.0 / _sizeM * math::rad2asec;
}


/**
 * @details
 * Zeros autocorrelations in the visibility matrix
 */
void ZenithImagerDft::_zeroAutoCorrelations(complex_t* vis, const unsigned nAnt)
{
    for (unsigned i = 0; i < nAnt; i++) {
        vis[i * nAnt + i] = complex_t(0.0, 0.0);
    }
}


/**
 * @details
 * Fill the visibility matrix with visibilities required for calculating the
 * point spread function.
 */
void ZenithImagerDft::_setPsfVisibilties(complex_t* vis, const unsigned nAnt)
{
    if (vis == NULL)
        throw QString("ZenithImagerDft::_setPsfVisibilties(): data not assigned");

    for (unsigned j = 0; j < nAnt; j++) {
        unsigned rowIndex = j * nAnt;
        for (unsigned i = 0; i < nAnt; i++) {
            unsigned index = rowIndex + i;
            if (i == j) vis[index] = complex_t(0.0, 0.0);
            else vis[index] = complex_t(1.0, 0.0);
        }
    }
}


} // namespace pelican
