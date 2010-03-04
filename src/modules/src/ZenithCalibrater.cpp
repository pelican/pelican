#include "modules/ZenithCalibrater.h"
#include "data/DataBlob.h"
#include "utility/ConfigNode.h"
#include "data/VisibilityData.h"
#include "data/ModelVisibilityData.h"
#include "data/CorrectedVisibilityData.h"
#include <QStringList>
#include "utility/pelicanTimer.h"
#include <iostream>

#include "utility/memCheck.h"


namespace pelican {

extern "C"{
void zheev_(const char* jobz, const char* uplo, int* n, complex_t* a, int* lda,
        double* w, complex_t* work, int* lwork, double* rwork, int* info);
}


/**
 * @details
 * Module constructor.
 */
ZenithCalibrater::ZenithCalibrater(const ConfigNode& config)
    : AbstractModule(config)
{
    _rawVis = NULL;
    _modelVis = NULL;
    _correctedVis = NULL;

    // Request the data blobs.
    addStreamData("VisibilityData");
    addGeneratedData("ModelVisibilityData");
    addGeneratedData("CorrectedVisibilityData");

    _getConfiguration(config);
}


/**
 * @details
 * Module destructor.
 */
ZenithCalibrater::~ZenithCalibrater()
{
}


/**
 * @details
 * Runs the calibration module using the supplied hash of data blobs.
 */
void ZenithCalibrater::run(QHash<QString, DataBlob*>& data)
{
    // Extract and check data blobs from the hash
    _fetchDataBlobs(data);

    unsigned nAnt = _rawVis->nAntennas();
    unsigned nChan = _channels.size();
    unsigned nPol = _polarisation == POL_BOTH ? 2 : 1;
    unsigned nVis = nAnt * nAnt;    // number of vis per chan per pol

    // zheev() work arrays.
    int lWork = 64 * nAnt;
    std::vector<complex_t> work(64 * nAnt);
    std::vector<double> rWork(3 * nAnt);

    // Temporary copies of visibility data matrices.
    std::vector<complex_t> Vz(nAnt * nAnt);
    std::vector<complex_t> modelTemp(nAnt * nAnt);

    // (name ??)
    std::vector<complex_t> Dz(nAnt, complex_t(0.0, 0.0));

    // Vector of complex antenna gains.
    std::vector<complex_t> gains(nAnt);

    // Sigma n (name ??)
    std::vector<complex_t> sigma_n(nAnt);

    for (unsigned c = 0; c < nChan; c++) {
        unsigned chan = _channels[c];

        for (unsigned p = 0; p < nPol; p++) {

            unsigned pol = (nPol = 1 && _polarisation == POL_X) ? 0 : 1;

            // Copy the input visibility data into the visibility work array
            complex_t* rawVis = _rawVis->ptr(chan, pol);
            complex_t* correctedVis = _correctedVis->ptr(chan, pol);
            complex_t* modelVis = _modelVis->ptr(c, pol);
            memcpy(&Vz[0], rawVis, nVis * sizeof(complex_t));
            memcpy(&modelTemp[0], modelVis, nVis * sizeof(complex_t));

            _calibrate(nAnt, _nEigenvaluesUsed, _nIterations, _tolerance,
                    lWork, &work[0], &rWork[0], &Vz[0], &Dz[0]);

            // Compute sigma_n.
            for (unsigned i = 0; i < nAnt; i++) {
                sigma_n[i] = rawVis[i + i * nAnt] - Dz[i];
            }

            _computeComplexGains(nAnt, _nEigenvaluesUsed, &Dz[0],
                    &modelTemp[0], &Vz[0], &work[0], lWork, &rWork[0],
                    &gains[0]);

            _buildCorrectedVisibilities(nAnt, rawVis, &gains[0], &sigma_n[0],
                    correctedVis);
        }
    }
}


/**
 * @details
 * For zheev() reference see: http://goo.gl/lPz1
 *
 * @param[in]     nAnt              Number of antennas.
 * @paran[in]     nEigenvaluesUsed  Number of eigenvalues retained per iteration.
 * @param[in]     nIterations       Maximum number of iterations in calibration loop.
 * @paran[in]     tolerance         Variable to determine if convergence has been met.
 * @param[in]     lWork             ??
 * @param[in]     work              ??
 * @param[in]     rWork             ??
 * @param[in,out] Vz                ??
 * @param[in,out] Dz                ??
 */
void ZenithCalibrater::_calibrate(
        const unsigned nAnt,
        const unsigned nEigenvaluesUsed,
        const unsigned nIterations,
        const double tolerance,
        int lWork,
        complex_t *work,
        double* rWork,
        complex_t* Vz,
        complex_t* Dz
){
    int info = 0;
    int n = nAnt;   // order of matrix A
    int lda = nAnt; // first dimension of the array a

    // Vector of eigenvalues produced in the calibration (all of them!)
    std::vector<double> eigenValues(nAnt);

    // Work array used in calculating new diagonals
    std::vector<complex_t> ww(nAnt * nEigenvaluesUsed);

    std::vector<complex_t> eigenVectors(nAnt * nAnt);

    double oldMaxEig = 0.0; // Previous max eigenvalue
    unsigned loopsReqired = 0;

    TIMER2_START
    /// Calibration interation
    for (unsigned k = 0; k < nIterations; k++) {
        // Set the diagonals of the vis work array
        _setDiagonals(Vz, nAnt, Dz);

        memcpy(&eigenVectors[0], Vz, nAnt * nAnt * sizeof(complex_t));

        // Find eigenvalues and eigenvectors
        zheev_("V", "U", &n, &eigenVectors[0], &lda, &eigenValues[0], work,
                &lWork, rWork, &info);

        if (info != 0) throw QString("_calibrate(): zheev() failed...");

        // Loop over eigenvalues in descending order and scale their
        // associated eigenVectors. (store the result in a row of ww)
        for (unsigned e = 0; e < nEigenvaluesUsed; e++) {
            unsigned index = nAnt - 1 - e;
            double eigenvalue = eigenValues[index];
            std::cout << e << ". eigenvalue = " << eigenvalue << std::endl;
            complex_t* eigenVector = &eigenVectors[nAnt * index];
            _sqrtScaleVector(eigenVector, nAnt, eigenvalue, &ww[e * nAnt]);
        }

        // Compute the new diagonal
        for (unsigned a = 0; a < nAnt; a++) {
            for (unsigned e = 0; e < nEigenvaluesUsed; e++) {
                unsigned index = a * nEigenvaluesUsed + e;
                Dz[a] = real(ww[index] * std::conj(ww[index]));
            }
        }

        // Check for convergence
        double maxEig = eigenValues[nAnt - 1];
        std::cout << std::fabs(maxEig - oldMaxEig) << std::endl;
        if (std::fabs(maxEig - oldMaxEig) <= tolerance * oldMaxEig) {
            loopsReqired = k+1;
            break;
        }
        else {
            oldMaxEig = maxEig;
            loopsReqired++;
        }
    }

    TIMER2_STOP("time taken")
    std::cout << "= Converged after " << loopsReqired << " loops." << std::endl;
}

/**
 * @details
 * Computes the complex antenna gain corrections.
 */
void ZenithCalibrater::_computeComplexGains(
        const unsigned nAnt,
        const unsigned /*ne*/,
        complex_t* Dz,
        complex_t* model,
        complex_t* Vz,
        complex_t* work,
        int lWork,
        double* rWork,
        complex_t* gains
){
    std::vector<double> zz(nAnt);
    std::vector<double> zs(nAnt);
    std::vector<double> dabs(nAnt);

    for (unsigned i = 0; i < nAnt; i++) {
        zz[i] = 1.0 / sqrt(Dz[i].real());
        zs[i] = 1.0 / sqrt(model[i * nAnt + i].real());
        dabs[i] = zz[i] / zs[i];
        Vz[i + nAnt * i] = Dz[i]; // changed from matlab (was +=)
    }

    for (unsigned j = 0; j < nAnt; j++) {
        for (unsigned i = 0; i < nAnt; i++) {
            const unsigned index = i + j * nAnt;
            model[index] *= std::conj(zs[j]) * zs[i];
            Vz[index] *= std::conj(zz[j]) * zz[i];
        }
    }

    int info = 0;
    int n = nAnt;   // order of matrix A
    int lda = nAnt; // first dimension of the array a
    std::vector<double> Ds(nAnt);
    std::vector<double> Dz2(nAnt);
    zheev_("V", "U", &n, model, &lda, &Ds[0], work, &lWork, rWork, &info);
    if (info != 0) throw QString("_calibrate(): zheev() failed...");

    zheev_("V", "U", &n, Vz, &lda, &Dz2[0], work, &lWork, rWork, &info);
    if (info != 0) throw QString("_calibrate(): zheev() failed...");


    for (unsigned i = 0; i < nAnt; i++) {
        const unsigned biggestIndex = nAnt - 1;
        complex_t top = Ds[biggestIndex] * model[nAnt * biggestIndex + i];
        complex_t bottom = Dz2[biggestIndex] * Vz[nAnt * biggestIndex + i];
        complex_t darg = top / bottom;
        gains[i] = dabs[i] * darg;
    }
}


/**
 * @details
 * Builds the corrected visibilities.
 */
void ZenithCalibrater::_buildCorrectedVisibilities(
        const unsigned nAnt,
        const complex_t* rawVis,
        const complex_t* gain,
        const complex_t* sigma_n,
        complex_t* correctedVis
){
    // Set the corrected visibilities to the raw visibilities
    for (unsigned j = 0; j < nAnt; j++) {
        for (unsigned i = 0; i < nAnt; i++){
            const unsigned index = j * nAnt + i;
            correctedVis[index] = rawVis[index];
        }
    }

    // Update the diagonals for sigma_n ?? (whatever sigma_n is...)
    for (unsigned j = 0; j < nAnt; j++) {
        const unsigned indexDiag = j * nAnt + j;
        correctedVis[indexDiag] = rawVis[indexDiag] - sigma_n[j];
    }

    // Construct the corrected visibilities.
    for (unsigned j = 0; j < nAnt; j++) {
        for (unsigned i = 0; i < nAnt; i++) {
            const unsigned index = j * nAnt + i;
            correctedVis[index] = gain[i] * correctedVis[index] * std::conj(gain[j]);
        }
    }
}


/**
 * @details
 * Sets the diagonals of a square matrix to the values given in a vector
 * of diagonals.
 */
void ZenithCalibrater::_setDiagonals(complex_t* matrix,
        const unsigned size, const complex_t* diagonals)
{
    for (unsigned i = 0; i < size; i++) {
        matrix[i * size + i] = diagonals[i];
    }
}


/**
 * @details
 * Scales a vector (\p v) by a given scale factor (\p value) and takes the
 * square root to return a scaled vector (\p scaled).
 */
void ZenithCalibrater::_sqrtScaleVector(complex_t* v, const unsigned size,
        const double value, complex_t* scaled)
{
    for (unsigned i = 0; i < size; i++) {
        scaled[i] = sqrt(value) * v[i];
    }
}


/**
 * @details
 */
void ZenithCalibrater::_fetchDataBlobs(QHash<QString, DataBlob*>& data)
{
    _rawVis = static_cast<VisibilityData*>(data["VisibilityData"]);
    _modelVis = static_cast<ModelVisibilityData*>(data["ModelVisibilityData"]);
    _correctedVis = static_cast<CorrectedVisibilityData*>(data["CorrectedVisibilityData"]);

    if (!_rawVis)
        throw QString("ZenithCalibrater: VisibilityData blob missing.");
    if (!_modelVis)
        throw QString("ZenithCalibrater: ModelVisibilityData blob missing.");
    if (!_correctedVis)
        throw QString("ZenithCalibrater: CorrectedVisibilityData blob missing.");

    if (_rawVis->nEntries() == 0)
        throw QString("ZenithCalibrater: Visibility data empty.");

    if (_channels.size() == 0)
            throw QString("ZenithCalibrater: No channels selected.");

    // Resize corrected visibilities.
    _correctedVis->resize(_rawVis->nAntennas(), _rawVis->nChannels(),
            _rawVis->nPolarisations());
}


/**
 * @details
 * Obtains the module configuration options from the XML file.
 */
void ZenithCalibrater::_getConfiguration(const ConfigNode& config)
{
    if (config.getDomElement().tagName() != "ZenithCalibrater")
        throw QString("ZenithCalibrater: Configuration missing.");

    _nEigenvaluesUsed = config.getOption("eigenvalues", "number").toUInt();
    _tolerance = config.getOption("tolerance", "value", "1.0e-4").toDouble();
    _nIterations = config.getOption("iterations", "value", "100").toUInt();

    if (_nEigenvaluesUsed == 0)
        throw QString("ZenithCalibrater: Have to select at least 1 eigenvalue to retain");

    _freqRefChannel = config.getOption("frequencies", "referenceChannel","0").toInt();
    _freqRef = config.getOption("frequencies", "reference", "0.0").toDouble();
    _freqDelta = config.getOption("frequencies", "delta", "195312.5").toDouble();

    // Get polarisation to calibrate.
    QString pol = config.getOption("polarisation", "value", "x").toLower();
    if (pol == "x") _polarisation = POL_X;
    else if (pol == "y") _polarisation = POL_Y;
    else if (pol == "both") _polarisation = POL_BOTH;

    // Get the channels to calibrate.
    QString chan = config.getOptionText("channels", "0");
    QStringList chanList = chan.split(",", QString::SkipEmptyParts);

    if (chanList.size() == 0)
        throw QString("ZenithCalibrater: No channels selected.");

    _channels.resize(chanList.size());
    for (int c = 0; c < chanList.size(); c++) {
        _channels[c] = chanList.at(c).toUInt();
    }
}

/**
 * @details
 */

} // namespace pelican
