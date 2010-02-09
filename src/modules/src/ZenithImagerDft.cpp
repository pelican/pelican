#include "modules/ZenithImagerDft.h"
#include "utility/constants.h"
#include <QString>
#include <QStringList>
#include <iostream>

#include "utility/memCheck.h"


namespace pelican {

/**
 * @details
 * Module constructor.
 */
ZenithImagerDft::ZenithImagerDft(const QDomElement& config)
    : AbstractModule(config)
{
    // Register which data blobs are needed by the module
    addRemoteStreamData("VisibilityData");
    addLocalStreamData("AntennaPositions");
    addLocalStreamData("ImageData");

    // Extract configuration from the xml configuration node.
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
ZenithImagerDft::~ZenithImagerDft()
{
}


/**
 * @details
 * Method called by the pipeline to create images of the visibility data.
 */
void ZenithImagerDft::run(QHash<QString, DataBlob*>& data)
{
    _fetchDataBlobs(data);
    unsigned nAnt = _antPos->nAntennas();
    unsigned nChan = _channels.size();
    unsigned nPol = _polarisation == POL_BOTH ? 2 : 1;

    /// Assign memory for the image (only resizes if needed)
    _image->assign(_sizeL, _sizeM, nChan, nPol);

    /// Loop over selected channels and polarisations to make images
    for (unsigned c = 0; c < nChan; c++) {
        unsigned channel = _channels[c];
        double frequency = _freq->at(channel);

        for (unsigned p = 0; p < nPol; p++) {

            unsigned pol = p;
            if (nPol == 1) pol = _polarisation;

            complex_t* vis = _vis->ptr(channel, pol);
            real_t* image = _image->ptr(pol, channel);
            _makeImageDft(nAnt, _antPos->xPtr(), _antPos->yPtr(), vis, frequency,
                    _sizeL, _sizeM, &_coordL[0], &_coordM[0], image);
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
void ZenithImagerDft::_getConfiguration(const QDomElement &config)
{
    _sizeL = getOption("imageSize", "l", "128").toUInt();
    _sizeM = getOption("imageSize", "m", "128").toUInt();
    _cellsizeL = getOption("cellsize", "l", "10.0").toDouble();
    _cellsizeM = getOption("cellsize", "m", "10.0").toDouble();

    QString pol = getOption("polarisation", "value", "x").toLower();
    if (pol == "x") _polarisation = POL_X;
    else if (pol == "y") _polarisation = POL_Y;
    else if (pol == "both") _polarisation = POL_BOTH;

    QString chan = getOptionText("channels", "0");
    QStringList chanList = chan.split(",", QString::SkipEmptyParts);
    _channels.resize(chanList.size());
    for (int c = 0; c < chanList.size(); c++) {
        _channels[c] = chanList.at(c).toUInt();
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
void ZenithImagerDft::_calculateImageCoords(const double cellsize,
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
 * Fetches named data blobs from the data blob hash into local local varibles.
 */
void ZenithImagerDft::_fetchDataBlobs(QHash<QString, DataBlob*>& data)
{
    _vis = static_cast<VisibilityData*>(data["VisibilityData"]);
    _antPos = static_cast<AntennaPositions*>(data["AntennaPositions"]);
    _image = static_cast<ImageData*>(data["ImageData"]);

    if (!_vis) throw QString("No visibility data,");
    if (!_antPos) throw QString("No antenna coordinate data");
    if (!_image) throw QString("No image data");
}


/**
 * @details
 * Calculates a matrix of complex weights for forming an image by
 * 2D DFT.
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
        const unsigned nL, const unsigned nM, real_t* coordsL, real_t* coordsM,
        real_t *image)
{
    std::vector<complex_t> weightsXL(nAnt * nL);
    std::vector<complex_t> weightsYM(nAnt * nM);
    _calculateWeights(nAnt, antPosX, frequency, nL, coordsL, &weightsXL[0]);
    _calculateWeights(nAnt, antPosY, frequency, nM, coordsM, &weightsYM[0]);

    std::vector<complex_t> weights(nAnt);
    std::vector<complex_t> temp(nAnt);

    for (unsigned m = 0; m < nM; m++) {
        complex_t *wYM = &weightsYM[m * nAnt];
        unsigned indexM = m * nL;

        for (unsigned l = 0; l < nL; l++) {
            complex_t * wXL = &weightsXL[l * nAnt];
            _multWeights(nAnt, wXL, wYM, &weights[0]);
            _multMatrixVector(nAnt, vis, &weights[0], &temp[0]);
            image[indexM + l] = _vectorDotConj(nAnt, &temp[0], &weights[0]).real();
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
#pragma omp parallel for
    for (unsigned i = 0; i < nAnt; i++) {
        weights[i] = weightsXL[i] * weightsYM[i];
    }
}


/**
 * @details
 * Performs matrix vector multiply of a matrix of visibility amplitudes
 * by a vector of complex dft weights
 */
void ZenithImagerDft::_multMatrixVector(const unsigned nAnt, complex_t* visMatrix,
        complex_t *weights, complex_t* result)
{
#pragma omp parallel for
    for (unsigned j = 0; j < nAnt; j++) {
        unsigned indexJ = j * nAnt;
        for (unsigned i = 0; i < nAnt; i++) {
            result[j] += visMatrix[indexJ + i] * weights[i];
        }
    }
}


/**
 * @details
 * Vector dot product
 */
complex_t ZenithImagerDft::_vectorDotConj(const unsigned n, complex_t* a, complex_t* b)
{
    complex_t result = complex_t(0.0, 0.0);
    for (unsigned i = 0; i < n; i++) {
        result += a[i] * conj(b[i]);
    }
    return result;
}



/**
 * @details
 */
void ZenithImagerDft::_cutHemisphere()
{
}


} // namespace pelican
