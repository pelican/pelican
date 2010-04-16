#include "pelican/modules/ZenithCalibrater.h"

#include "pelican/data/DataBlob.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/data/VisibilityData.h"
#include "pelican/data/ModelVisibilityData.h"
#include "pelican/data/CorrectedVisibilityData.h"
#include "pelican/utility/pelicanTimer.h"

#include <QStringList>
#include <fstream>
#include <cblas.h>
#include <iostream>
#include <iomanip>

#include "pelican/utility/memCheck.h"

using std::cout;
using std::endl;
using std::vector;
using std::conj;

namespace pelican {

PELICAN_DECLARE_MODULE(ZenithCalibrater)

/**
 * @details
 * Module constructor. Register data blobs required by the module and get the
 * configuration options from the XML node.
 */
ZenithCalibrater::ZenithCalibrater(const ConfigNode& config)
    : AbstractModule(config)
{
    // Retrieve configuration options from the XML node.
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
void ZenithCalibrater::run(VisibilityData* _rawVis,
        ModelVisibilityData* _modelVis, CorrectedVisibilityData* _correctedVis)
{
    // Extract and check data blobs from the hash.
    _checkDataBlobs(_rawVis, _modelVis, _correctedVis);

    // Resize corrected visibilities.
    _correctedVis->resize(_rawVis->nAntennas(), _channels, _polarisation);

    /// Data dimensions.
    unsigned nAnt = _rawVis->nAntennas();
    unsigned nVis = nAnt * nAnt;

    // zheev() work arrays.
    int lWork = 64 * nAnt;
    vector<complex_t> work(64 * nAnt);
    vector<double> rWork(3 * nAnt);

    // Temporary copies of visibility data matrices.
    vector<complex_t> Vz(nAnt * nAnt);
    vector<complex_t> modelTemp(nAnt * nAnt);

    // (name ??)
    vector<complex_t> Dz(nAnt, complex_t(0.0, 0.0));

    // Sigma n (name ??).
    vector<complex_t> sigma_n(nAnt);

    // Vector of complex antenna gains found by the calibration.
    vector<complex_t> gains(nAnt);

    // Number of channels & polarisations to calibrate.
    unsigned nChanCal = _channels.size();
    unsigned nPolCal = _polarisation == POL_BOTH ? 2 : 1;

    // Check input data and selection polarisation for consistency.
    checkPolarisationConsitency(_rawVis->polarisation(), _polarisation);
    checkPolarisationConsitency(_modelVis->polarisation(), _polarisation);
    checkPolarisationConsitency(_correctedVis->polarisation(), _polarisation);

    // Loop over selected channels and polarisations and calibrate.
    for (unsigned c = 0; c < nChanCal; c++) {

        unsigned chanSelected = _channels[c];
        // Find out if the selected channel is available in the data and if so
        // get its index.
        unsigned iChanRaw = findIndex(chanSelected, _rawVis->channels());
        unsigned iChanModel = findIndex(chanSelected, _modelVis->channels());
        unsigned iChanCorected = findIndex(chanSelected, _correctedVis->channels());

        for (unsigned p = 0; p < nPolCal; p++) {

            unsigned iPol = (nPolCal == 1 &&
                    _polarisation == POL_X) ? 0 : 1;

            // Copy the input visibility data into the visibility work array
            complex_t* rawVis = _rawVis->ptr(iChanRaw, iPol);
            if (rawVis == NULL)
                throw QString("ZenithCalibrater: Raw visibility data missing for selected channel and polarisation");

            complex_t* correctedVis = _correctedVis->ptr(iChanCorected, p);
            if (correctedVis == NULL)
                throw QString("ZenithCalibrater: Corrected visibility data missing for selected channel and polarisation");

            complex_t* modelVis = _modelVis->ptr(iChanModel, p);
            if (correctedVis == NULL)
                throw QString("ZenithCalibrater: Model visibility data missing for selected channel and polarisation");

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
 * @param[in]     nEigenvaluesUsed  Number of eigenvalues retained per iteration.
 * @param[in]     nIterations       Maximum number of iterations in calibration loop.
 * @param[in]     tolerance         Variable to determine if convergence has been met.
 * @param[in]     lWork             ??
 * @param[in]     work              ??
 * @param[in]     rWork             ??
 * @param[in,out] Vz                ??
 * @param[in,out] Dz                ??
 *
 * TODO:
 * = Return number of iterations to calibrate into some history...
 * = Better handling of zheev() info parameter.
 * = Non cblas fallback functions... ?
 */
void ZenithCalibrater::_calibrate(const unsigned nAnt,
        const unsigned nEigenvaluesUsed, const unsigned nIterations,
        const double tolerance, int lWork, complex_t *work, double* rWork,
        complex_t* Vz, complex_t* Dz)
{
    int info = 0;
    int n = nAnt;   // order of matrix A
    int lda = nAnt; // first dimension of the array a

    // Vector of eigenvalues produced in the calibration (all of them!)
    vector<double> eigenValues(nAnt);

    // Work array used in calculating new diagonals
    vector<complex_t> ww(nAnt * nEigenvaluesUsed);

    /// Matrix of eigenvalues produced by zheev() [note: column major order].
    vector<complex_t> eigenVectors(nAnt * nAnt);

    // Previous max eigenvalue used for termination condition.
    double oldMaxEigenvalue = 0.0;

    /// Calibration iteration.
    for (unsigned k = 0; k < nIterations; k++) {

        // Set the diagonals of the vis work array Vz to vector Dz.
        _setDiagonals(nAnt, Dz, Vz);

        // Copy the updated visibility matrix into the eigenvector matrix.
        memcpy(&eigenVectors[0], Vz, nAnt * nAnt * sizeof(complex_t));

        // Compute the eigenvalues and eigenvectors.
        zheev_("V", "U", &n, &eigenVectors[0], &lda, &eigenValues[0], work,
                &lWork, rWork, &info);
        if (info != 0)
            throw QString("_calibrate(): zheev() failed (info = %1)").arg(info);

        // Scale the largest \p nEigenvaluesUsed eigenvectors by their
        // associated eigenvalues storing the result in rows of ww.
        for (unsigned e = 0; e < nEigenvaluesUsed; e++) {
            unsigned j = nAnt - e - 1;
            cblas_zcopy(nAnt, &eigenVectors[j * nAnt], 1, &ww[e * nAnt], 1);
            cblas_zdscal(nAnt, sqrt(eigenValues[j]), &ww[e * nAnt], 1);
        }

        // Compute the new visibility diagonal from the scaled eigenvectors.
        complex_t compTemp;
        for (unsigned a = 0; a < nAnt; a++) {
            cblas_zdotc_sub(nEigenvaluesUsed, &ww[a], nAnt, &ww[a], nAnt, &compTemp);
            Dz[a] = compTemp.real();
        }

        // Check for convergence.
        double maxEigenvalue = eigenValues[nAnt - 1];
        if (fabs(maxEigenvalue - oldMaxEigenvalue) <= tolerance * oldMaxEigenvalue) {
            break;
        }
        else {
            oldMaxEigenvalue = maxEigenvalue;
        }
    }
}


/**
 * @details
 * Computes the complex antenna gain corrections.
 *
 * @param[in]  nAnt             Number of antennas.
 * @param[in]  nEigenvaluesUsed Number of eigenvalues used in calibration.
 * @param[in]  Dz               ??
 * @param[in]  modelVisTemp     Temporary copy of the model visibilities.
 * @param[in]  Vz               ??
 * @param[in]  work             ?? (zheev() work array)
 * @param[in]  lWork            ??
 * @param[in]  rWork            ??
 * @param[out] gains            Complex antenna gains produced by the calibration.
 *
 * TODO:
 *  = Better checking on info return by zheev()?
 *  = Optimise use of temp arrays
 */
void ZenithCalibrater::_computeComplexGains(
        const unsigned nAnt,
        const unsigned /* nEigenvaluesUsed */,
        complex_t* Dz,
        complex_t* modelVisTemp,
        complex_t* Vz,
        complex_t* work,
        int lWork,
        double* rWork,
        complex_t* gains
){
    vector<double> zz(nAnt);
    vector<double> zs(nAnt);
    vector<double> dabs(nAnt);

    for (unsigned i = 0; i < nAnt; i++) {
        zz[i] = 1.0 / sqrt(Dz[i].real());
        zs[i] = 1.0 / sqrt(modelVisTemp[i * nAnt + i].real());
        dabs[i] = zz[i] / zs[i];
        Vz[i + nAnt * i] = Dz[i]; // changed from matlab (was +=)
    }

    for (unsigned j = 0; j < nAnt; j++) {
        for (unsigned i = 0; i < nAnt; i++) {
            const unsigned index = i + j * nAnt;
            modelVisTemp[index] *= (zs[j] * zs[i]);
            Vz[index] *= (zz[j] * zz[i]);
        }
    }

    int info = 0;
    int n = nAnt;   // order of matrix A
    int lda = nAnt; // first dimension of the array a
    vector<double> Ds(nAnt);
    vector<double> Dz2(nAnt);
    zheev_("V", "U", &n, modelVisTemp, &lda, &Ds[0], work, &lWork, rWork, &info);
    if (info != 0)
        throw QString("_calibrate(): zheev() failed (info = %1)").arg(info);

    zheev_("V", "U", &n, Vz, &lda, &Dz2[0], work, &lWork, rWork, &info);
    if (info != 0)
        throw QString("_calibrate(): zheev() failed (info = %1)").arg(info);

    for (unsigned i = 0; i < nAnt; i++) {
        const unsigned biggestIndex = nAnt - 1;
        complex_t top = Ds[biggestIndex] * modelVisTemp[nAnt * biggestIndex + i];
        complex_t bottom = Dz2[biggestIndex] * Vz[nAnt * biggestIndex + i];
        complex_t darg = top / bottom;
        gains[i] = dabs[i] * darg;
    }
}


/**
 * @details
 * Builds the corrected visibilities.
 *
 * @param[in]  nAnt         Number of antenna.
 * @param[in]  rawVis       Matrix of raw (measured) visibilities.
 * @param[in]  gains        Vector of antenna gains produced by the calibration.
 * @param[in]  signa_n      ??
 * @param[out] correctedVis Matrix of visibilties corrected by the calibration.
 */
void ZenithCalibrater::_buildCorrectedVisibilities(const unsigned nAnt,
        const complex_t* rawVis, const complex_t* gains,
        const complex_t* sigma_n, complex_t* correctedVis)
{
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
            correctedVis[index] = gains[i] * correctedVis[index] * conj(gains[j]);
        }
    }
}


/**
 * @details
 * Sets the diagonals of a square matrix to the values given in a vector
 * of diagonals.
 *
 * @param[in]     size       Size of the matrix (square matrix assumed).
 * @param[in]     diagonals  Vector of diagonals used to update the matrix.
 * @param[in,out] matrix     Matrix of complex values for which the diagonals
 *                           are to be updated.
 */
void ZenithCalibrater::_setDiagonals(const unsigned size,
        const complex_t* diagonals, complex_t* matrix)
{
    for (unsigned i = 0; i < size; i++) {
        matrix[i * size + i] = diagonals[i];
    }
}


/**
 * @details
 */
void ZenithCalibrater::_checkDataBlobs(VisibilityData* rawVis,
        ModelVisibilityData* modelVis,
        CorrectedVisibilityData* correctedVis)
{
    if (!rawVis)
        throw QString("ZenithCalibrater: VisibilityData blob missing.");
    if (!modelVis)
        throw QString("ZenithCalibrater: ModelVisibilityData blob missing.");
    if (!correctedVis)
        throw QString("ZenithCalibrater: CorrectedVisibilityData blob missing.");

    if (rawVis->nEntries() == 0)
        throw QString("ZenithCalibrater: Visibility data empty.");

    if (_channels.size() == 0)
            throw QString("ZenithCalibrater: No channels selected.");
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

    // Get the channel and polarisation selection.
    _channels = config.getChannels();
    _polarisation = config.getPolarisation();
}


} // namespace pelican
