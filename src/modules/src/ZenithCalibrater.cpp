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
void zheev_(char* jobz, char* uplo, int* n, complex_t* a, int* lda, double* w,
        complex_t* work, int* lwork, double* rwork, int* info);
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
 * @detail
 * Runs the module
 */
void ZenithCalibrater::run(QHash<QString, DataBlob*>& data)
{
    // Extract and check data blobs from the hash
    _fetchDataBlobs(data);

    unsigned nAnt = _vis->nAntennas();
    unsigned nChan = _channels.size();
    unsigned nPol = _polarisation == POL_BOTH ? 2 : 1;
    unsigned nVis = nAnt * nAnt;    // number of vis per chan per pol

    // Visibility data work array used in the calibration iteration.
    std::vector<complex_t> visTemp(nAnt * nAnt);

    // ?? work array
    std::vector<complex_t> ww(nAnt * _nEigenvalues);

    // Auto-correlations work array
    std::vector<complex_t> autoCorrelations(nAnt, complex_t(0.0, 0.0));

    for (unsigned c = 0; c < nChan; c++) {
        unsigned chan = _channels[c];

        for (unsigned p = 0; p < nPol; p++) {
            unsigned pol = (nPol = 1 && _polarisation == POL_X) ? 1 : 2;

            // Copy the input visibility data into the visibility work array
            memcpy(&visTemp[0], _vis->ptr(chan, pol), nVis * sizeof(complex_t));

            _calibrate(&visTemp[0], nAnt, &autoCorrelations[0],
                    &ww[0], _nEigenvalues);
        }
    }
}


/**
 * @details
 * For reference on zheev() see: http://goo.gl/lPz1
 */
void ZenithCalibrater::_calibrate(complex_t* visTemp, const unsigned nAnt,
        complex_t* autoCorrelations, complex_t* ww,
        const unsigned nEigenvalues)
{
    TIMER2_START

    // zheev paramters and initialisation to find optimal workspace
    // (move this outside the function or use 64 * nAnt for lWork size
    // =========================================================================
    int n = nAnt;   // order of matrix A
    int lda = nAnt; // first dimension of the array a
    int info = 0;
    int lWork = -1;
    complex_t optimalWorkspace;
    std::vector<double> rWork(3 * nAnt - 2); // Workspace array
    std::vector<double> w(nAnt); // eigenvalues in ascending order

    // Query and allocate the optimal workspace. If lWork = -1, the first value
    // of work is the size of the optimal work array.
    zheev_("Vectors","Lower", &n, visTemp, &lda, &w[0], &optimalWorkspace,
            &lWork, &rWork[0], &info);

    lWork = static_cast<int>(optimalWorkspace.real());
    std::cout << lWork << std::endl;

    std::vector<complex_t> work(lWork);
    // =========================================================================

//    return;

    /// Calibration interation
    for (unsigned k = 0; k < 1; k++) {

        // Set the diagonals of the vis work array
        _setDiagonals(visTemp, nAnt, autoCorrelations);

        // Find eigenvalues and eigenvectors
        zheev_("Vectors", "Lower", &n, &visTemp[0], &lda, &w[0], &work[0], &lWork, &rWork[0],
                &info);

//        std::cout << "info " << info << std::endl;

        // ?? step
        for (unsigned j = 0; j < nEigenvalues; j++) {
//            double eigenValue = w[nEigenvalues- 1 - j];
            double eigenValue = w[j];
            std::cout << j << ". eigenvalue = " << eigenValue << std::endl;
//            std::cout << "------------------------------\n";
//            for (unsigned i = 0; i < nAnt; i++) {
//                std::cout << visTemp[j * nAnt + i].real() << std::endl;
//////                ww[j * nAnt + i] = w[nEigenvalues- 1 - j] *
//            }
        }

        // Compute new diagonal (autocorrelation)
        for (unsigned j = 0; j < nAnt; j++) {
            // form new autocorrelations
        }

        // Check convergence


    }

    TIMER2_STOP("_calibrate")
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
 * @detail
 * Grab the configuration from XML config node
 */
void ZenithCalibrater::_getConfiguration(const ConfigNode& config)
{
    _nEigenvalues = config.getOption("eigenvalues", "number", "10").toUInt();
    _tolerance = config.getOption("tolerance", "value", "1.0e-4").toDouble();
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
    _channels.resize(chanList.size());
    for (int c = 0; c < chanList.size(); c++) {
        _channels[c] = chanList.at(c).toUInt();
    }

}


} // namespace pelican
