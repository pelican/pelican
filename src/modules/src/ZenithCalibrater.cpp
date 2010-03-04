#include "modules/ZenithCalibrater.h"
#include "data/DataBlob.h"
#include "utility/ConfigNode.h"
#include "data/VisibilityData.h"
#include "data/AntennaPositions.h"
#include "data/ModelVisibilityData.h"
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
    _vis = NULL;
    _modelVis = NULL;
    _antPos = NULL;
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

    unsigned nAnt = _vis->nAntennas();
    nAnt = 3;
    _nEigenvalues = nAnt;
    unsigned nChan = _channels.size();
    unsigned nPol = _polarisation == POL_BOTH ? 2 : 1;
    unsigned nVis = nAnt * nAnt;    // number of vis per chan per pol

    // Visibility data work array used in the calibration iteration.
    std::vector<complex_t> visTemp(nAnt * nAnt);

    // ?? work array
    std::vector<complex_t> ww(nAnt * _nEigenvalues);

    // Auto-correlations work array
    std::vector<complex_t> autoCorrelations(nAnt, complex_t(0.0, 0.0));

    // zheev() Workspace array
    std::vector<double> rWork(3 * nAnt);

    // Vector of eigenvalues produced in the calibration (all of them!)
    std::vector<double> eigenValues(nAnt);

    // Allocate optimal zheev() work array.
    int info = 0;
    int n = nAnt;   // order of matrix A
    int lda = nAnt; // first dimension of the array a
    int lWork = -1;
    complex_t optWork;
    zheev_("V", "U", &n, &visTemp[0], &lda, &eigenValues[0], &optWork, &lWork,
            &rWork[0], &info);
    lWork = static_cast<int>(optWork.real());
    std::vector<complex_t> work(lWork);


    visTemp[0] = 3;
    visTemp[1] = 2;
    visTemp[2] = -1;

    visTemp[3] = 2;
    visTemp[4] = 1;
    visTemp[5] = 0;

    visTemp[6] = -1;
    visTemp[7] = 0;
    visTemp[8] = 1;

//    visTemp[0] = 0;
//    visTemp[1] = 1;
//    visTemp[2] = -5;
//
//    visTemp[3] = 2;
//    visTemp[4] = 1;
//    visTemp[5] = 0;
//
//    visTemp[6] = -1;
//    visTemp[7] = 0;
//    visTemp[8] = 1;




    for (unsigned j = 0; j < nAnt; j++) {
        for (unsigned i = 0; i < nAnt; i++) {
//            visTemp[j * nAnt + i] = complex_t(nAnt - 1 - j, 0.0);
//            visTemp[j * nAnt + i] = complex_t(nAnt - 1 - i, 0.0);
//            visTemp[j * nAnt + i] = complex_t(j, 0.0);
//            visTemp[j * nAnt + i] = complex_t(i, 0.0);
            std::cout << visTemp[j * nAnt + i] << " ";
        }
        std::cout << std::endl;
    }


    _calibrate(&visTemp[0], nAnt, &autoCorrelations[0], &ww[0],
            &rWork[0], &eigenValues[0], _nEigenvalues, &work[0], lWork,
            _iterations);


//
//    for (unsigned c = 0; c < nChan; c++) {
//        unsigned chan = _channels[c];
//
//        for (unsigned p = 0; p < nPol; p++) {
//            unsigned pol = (nPol = 1 && _polarisation == POL_X) ? 0 : 1;
//
//            // Copy the input visibility data into the visibility work array
//            complex_t* vis = _vis->ptr(chan, pol);
//            memcpy(&visTemp[0], vis, nVis * sizeof(complex_t));
//
//            for (unsigned j = 0; j < nAnt; j++) {
//                for (unsigned i = 0; i < nAnt; i++) {
//                    visTemp[j * nAnt + i] = complex_t(1.0, 0.0);
//                }
//            }
//
//            _calibrate(&visTemp[0], nAnt, &autoCorrelations[0], &ww[0],
//                    &rWork[0], &eigenValues[0], _nEigenvalues, &work[0], lWork,
//                    _iterations);
//
//        }
//    }
}


/**
 * @details
 * For reference on zheev() see: http://goo.gl/lPz1
 */
void ZenithCalibrater::_calibrate(complex_t* visTemp, const unsigned nAnt,
        complex_t* autoCorrelations, complex_t* ww, double* rWork,
        double* eigenValues, const unsigned nEigenvaluesUsed, complex_t *work,
        int lWork, const unsigned nIter)
{
    int info = 0;
    int n = nAnt;   // order of matrix A
    int lda = nAnt; // first dimension of the array a

    if (visTemp == NULL || autoCorrelations == NULL || ww == NULL ||
            rWork == NULL || eigenValues == NULL || work == NULL)
        throw QString("AAAAA");

    /// Calibration interation
    for (unsigned k = 0; k < nIter; k++) {
        // Set the diagonals of the vis work array
//        _setDiagonals(visTemp, nAnt, autoCorrelations);

        // Find eigenvalues and eigenvectors
        zheev_("V", "U", &n, visTemp, &lda, eigenValues, work, &lWork,
                rWork, &info);
        std::cout << "info = " << info << std::endl;

        // print eigenvalues
        for (unsigned e = 0; e < nEigenvaluesUsed; e++) {
//            unsigned index = nAnt - 1 - e;
            unsigned index = e;
            std::cout << index << ". eigenvalue = " << eigenValues[index] << std::endl;
            std::cout << "-----------------------" << std::endl;
            for (unsigned a = 0; a < nAnt; a++) {
                unsigned vindex = e * nAnt + a;

                std::cout << vindex << " " << visTemp[vindex] << std::endl;
            }

        }

        // TODO: Compute new diagonal (autocorrelation)

        // TODO: Check convergence
    }
}

/**
 * @details
 * Computes the complex antenna gain corrections.
 */
void ZenithCalibrater::_computeComplexGains (
        const unsigned n_a,
        const unsigned ne,
        double* Dz,
        complex_t* model,
        complex_t* Vz,
        complex_t* work,
        int lWork,
        double* rWork,
        complex_t* gains
){
    std::vector<double> zz(n_a);
    std::vector<double> zs(n_a);
    std::vector<double> dabs(n_a);
    for (unsigned i = 0; i < n_a; i++) {
        zz[i] = 1.0 / sqrt(Dz[i]);
        zs[i] = 1.0 / sqrt(model[i * n_a + i].real());
        dabs[i] = zz[i] / zs[i];
        Vz[i + n_a + i] += Dz[i];
    }

    for (unsigned j = 0; j < n_a; j++) {
        for (unsigned i = 0; i < n_a; i++) {
            const unsigned index = i + j * n_a;
            model[index] *= std::conj(zs[j]) * zs[i];
            Vz[index] *= std::conj(zz[j]) * zz[i];
        }
    }

    int info = 0;
    int n = n_a;   // order of matrix A
    int lda = n_a; // first dimension of the array a
    std::vector<double> Ds(n_a);
    std::vector<double> Dz2(n_a);
    zheev_("V", "U", &n, model, &lda, &Ds[0], work, &lWork,
                    rWork, &info);

    zheev_("V", "U", &n, Vz, &lda, &Dz2[0], work, &lWork,
                    rWork, &info);

    for (unsigned i = 0; i < n_a; i++) {
        const unsigned biggest = n_a - 1;
        complex_t top = Ds[biggest] * model[n_a * biggest + i];
        complex_t bottom = Dz2[biggest] * Vz[n_a * biggest + i];
        complex_t darg = top / bottom;
        gains[i] = dabs[i] * darg;
    }
}

/**
 * @details
 * Builds the corrected visibilities.
 */
void ZenithCalibrater::_buildCorrectedVisibilities (
        const unsigned n_a,
        const complex_t* vis,
        const complex_t* gains,
        const complex_t* sigma_n,
        complex_t* visCorrected
){
    // Loop over columns in the visibility matrix.
    for (unsigned j = 0; j < n_a; j++)
    {
        // Copy the column.
        for (unsigned i = 0; i < n_a; i++)
        {
            const unsigned index = i + j * n_a;
            visCorrected[index] = vis[index];
        }

        // Set the matrix diagonal.
        const unsigned indexDiag = j + j * n_a;
        visCorrected[indexDiag] = vis[indexDiag] - sigma_n[j];

        // Multiply by the gains.
        const complex_t gain = gains[j];
        for (unsigned i = 0; i < n_a; i++)
        {
            const unsigned index = i + j * n_a;
            visCorrected[index] = gain * visCorrected[index] * std::conj(gain);
        }
    }
}

/**
 * @details
 */
void ZenithCalibrater::_zeroDiagonals(complex_t* matrix, const unsigned size)
{
    for (unsigned i = 0; i < size; i++) {
        matrix[i * size + i] = 0.0;
    }
}


/**
 * @details
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
 */
void ZenithCalibrater::_fetchDataBlobs(QHash<QString, DataBlob*>& data)
{
    _vis = static_cast<VisibilityData*>(data["VisibilityData"]);
    _modelVis = static_cast<ModelVisibilityData*>(data["ModelVisibilityData"]);
    _antPos = static_cast<AntennaPositions*>(data["AntennaPositions"]);

    if (!_vis)
        throw QString("ZenithCalibrater: VisibilityData blob missing.");
    if (!_modelVis)
        throw QString("ZenithCalibrater: ModelVisibilityData blob missing.");
    if (!_antPos)
        throw QString("ZenithCalibrater: AntennaPositions blob missing.");

    if (_antPos->nAntennas() == 0)
        throw QString("ZenithCalibrater: No antennas found.");

    if (_vis->nEntries() == 0)
        throw QString("ZenithCalibrater: Visibility data empty.");

    if (_vis->nAntennas() != _antPos->nAntennas())
        throw QString("ZenithCalibrater: Antenna dimension mismatch");

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

    _nEigenvalues = config.getOption("eigenvalues", "number").toUInt();
    _tolerance = config.getOption("tolerance", "value", "1.0e-4").toDouble();
    _iterations = config.getOption("iterations", "value", "100").toUInt();

    if (_nEigenvalues == 0)
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
