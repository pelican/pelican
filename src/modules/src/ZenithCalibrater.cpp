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
    _vis = NULL;
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

    unsigned nAnt = _vis->nAntennas();
    //nAnt = 3;
    //_nEigenvalues = nAnt;
    unsigned nChan = _channels.size();
    unsigned nPol = _polarisation == POL_BOTH ? 2 : 1;
    unsigned nVis = nAnt * nAnt;    // number of vis per chan per pol

    // Visibility data work array used in the calibration iteration.
    std::vector<complex_t> visTemp(nAnt * nAnt);
    std::vector<complex_t> modelTemp(nAnt * nAnt);

    // ?? work array
    std::vector<complex_t> ww(nAnt * _nEigenvalues);

    // Auto-correlations work array
    std::vector<complex_t> autoCorrelations(nAnt, complex_t(0.0, 0.0));

    // zheev() Workspace array
    std::vector<double> rWork(3 * nAnt);

    // Vector of eigenvalues produced in the calibration (all of them!)
    std::vector<double> eigenValues(nAnt);

    // Vector of complex antenna gains.
    std::vector<complex_t> gains(nAnt);

    // Sigma n
    std::vector<complex_t> sigma_n(nAnt);

    // Allocate optimal zheev() work array.
    int info = 0;
    int n = nAnt;   // order of matrix A
    int lda = nAnt; // first dimension of the array a
    int lWork = -1;
    complex_t optWork;
    zheev_("V", "U", &n, &visTemp[0], &lda, &eigenValues[0], &optWork, &lWork,
            &rWork[0], &info);
    if (info != 0) throw QString("_calibrate(): zheev() failed...");
    lWork = static_cast<int>(optWork.real());
    std::cout << "Lwork: " << lWork << std::endl;
    std::vector<complex_t> work(lWork);


//    visTemp[0] = 3;
//    visTemp[1] = 2;
//    visTemp[2] = -1;
//
//    visTemp[3] = 2;
//    visTemp[4] = 1;
//    visTemp[5] = 0;
//
//    visTemp[6] = -1;
//    visTemp[7] = 0;
//    visTemp[8] = 1;

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

//    for (unsigned j = 0; j < nAnt; j++) {
//        for (unsigned i = 0; i < nAnt; i++) {
////            visTemp[j * nAnt + i] = complex_t(nAnt - 1 - j, 0.0);
////            visTemp[j * nAnt + i] = complex_t(nAnt - 1 - i, 0.0);
////            visTemp[j * nAnt + i] = complex_t(j, 0.0);
////            visTemp[j * nAnt + i] = complex_t(i, 0.0);
//            std::cout << visTemp[j * nAnt + i] << " ";
//        }
//        std::cout << std::endl;
//    }
//
//
//    _calibrate(&visTemp[0], nAnt, &autoCorrelations[0], &ww[0],
//            &rWork[0], &eigenValues[0], _nEigenvalues, &work[0], lWork,
//            _iterations, _tolerance);


    for (unsigned c = 0; c < nChan; c++) {
        unsigned chan = _channels[c];

        for (unsigned p = 0; p < nPol; p++) {
            unsigned pol = (nPol = 1 && _polarisation == POL_X) ? 0 : 1;

            // Copy the input visibility data into the visibility work array
            complex_t* vis = _vis->ptr(chan, pol);
            complex_t* visModel = _modelVis->ptr(0, 0);
            complex_t* visCorrected = _correctedVis->ptr(chan, pol);

            memcpy(&visTemp[0], vis, nVis * sizeof(complex_t));
            memcpy(&modelTemp[0], visModel, nVis * sizeof(complex_t));

//            for (unsigned j = 0; j < nAnt; j++) {
//                for (unsigned i = 0; i < nAnt; i++) {
//                    visTemp[j * nAnt + i] = complex_t(1.0, 0.0);
//                }
//            }

            _calibrate(&visTemp[0], nAnt, &autoCorrelations[0], &ww[0],
                    &rWork[0], &eigenValues[0], _nEigenvalues, &work[0], lWork,
                    _iterations, _tolerance);

            // Compute sigma_n.
            for (unsigned i = 0; i < nAnt; i++) {
                sigma_n[i] = vis[i + i * nAnt] - autoCorrelations[i];
            }

            _computeComplexGains(nAnt, _nEigenvalues, &autoCorrelations[0], &modelTemp[0], &visTemp[0], &work[0], lWork, &rWork[0], &gains[0]);
//            _computeComplexGains(nAnt, _nEigenvalues, &autoCorrelations[0], visModel, &visTemp[0], &work[0], lWork, &rWork[0], &gains[0]);

            _buildCorrectedVisibilities(nAnt, vis, &gains[0], &sigma_n[0], visCorrected);
        }
    }
//    std::cout << "Here" << std::endl;

}


/**
 * @details
 * For reference on zheev() see: http://goo.gl/lPz1
 */
void ZenithCalibrater::_calibrate(complex_t* visTemp, const unsigned nAnt,
        complex_t* autoCorrelations, complex_t* ww, double* rWork,
        double* eigenValues, const unsigned nEigenvaluesUsed, complex_t *work,
        int lWork, const unsigned nIter, const double tolerance)
{
    int info = 0;
    int n = nAnt;   // order of matrix A
    int lda = nAnt; // first dimension of the array a

    if (visTemp == NULL || autoCorrelations == NULL || ww == NULL ||
            rWork == NULL || eigenValues == NULL || work == NULL)
        throw QString("AAAAA");

    double oldMaxEig = 0.0; // Previous max eigenvalue
    unsigned loopsReqired = 0;

    std::vector<complex_t> Vz(nAnt * nAnt);
    memcpy(&Vz[0], visTemp, nAnt * nAnt * sizeof(complex_t));

    TIMER2_START
    /// Calibration interation
    for (unsigned k = 0; k < nIter; k++) {
        // Set the diagonals of the vis work array
        _setDiagonals(&Vz[0], nAnt, autoCorrelations);

        memcpy(visTemp, &Vz[0], nAnt * nAnt * sizeof(complex_t));

        // Find eigenvalues and eigenvectors
        zheev_("V", "U", &n, visTemp, &lda, eigenValues, work, &lWork,
                rWork, &info);
        if (info != 0) throw QString("_calibrate(): zheev() failed...");

        // print eigenvalues
//        for (unsigned e = 0; e < nEigenvaluesUsed; e++) {
//            unsigned index = nAnt - 1 - e;
//            unsigned index = e;
//            std::cout << index << ". eigenvalue = " << eigenValues[index] << std::endl;
//            std::cout << "-----------------------" << std::endl;
//            for (unsigned a = 0; a < nAnt; a++) {
//                unsigned vindex = e * nAnt + a;
//                std::cout << vindex << " " << visTemp[vindex] << std::endl;
//        }


        // Loop over eigenvalues in descending order and scale their
        // associated eigenVectors. (store the result in a row of ww)
        for (unsigned e = 0; e < nEigenvaluesUsed; e++) {
            unsigned index = nAnt - 1 - e;
            double eigenvalue = eigenValues[index];
            std::cout << index << ". eigenvalue = " << eigenvalue << std::endl;
            complex_t* eigenVector = &visTemp[nAnt * index];
            complex_t* scaledEigenVector = &ww[e * nAnt];
            _sqrtScaleVector(eigenVector, nAnt, eigenvalue, scaledEigenVector);
        }

        // Compute the new diagonal
        for (unsigned a = 0; a < nAnt; a++) {
            for (unsigned e = 0; e < nEigenvaluesUsed; e++) {
                unsigned index = a * nEigenvaluesUsed + e;
                autoCorrelations[a] = real(ww[index] * std::conj(ww[index]));
            }
        }

        // Check for convergence
        double maxEig = eigenValues[nAnt - 1];
        std::cout << std::fabs(maxEig - oldMaxEig) << std::endl;
        if (std::fabs(maxEig - oldMaxEig) <= tolerance * oldMaxEig) {
            loopsReqired = k;
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
void ZenithCalibrater::_computeComplexGains (
        const unsigned n_a,
        const unsigned ne,
        complex_t* Dz,
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
        zz[i] = 1.0 / sqrt(Dz[i].real());
        zs[i] = 1.0 / sqrt(model[i * n_a + i].real());
        dabs[i] = zz[i] / zs[i];
        Vz[i + n_a * i] = Dz[i]; // changed from matlab (was +=)
    }

    std::cout << "zz[0] = " << zz[0] << std::endl;
    std::cout << "Vz[0] = " << Vz[0] << std::endl;

    for (unsigned j = 0; j < n_a; j++) {
        for (unsigned i = 0; i < n_a; i++) {
            const unsigned index = i + j * n_a;
            model[index] *= std::conj(zs[j]) * zs[i];
            Vz[index] *= std::conj(zz[j]) * zz[i];
//            model[index] = Vz[index];
        }
    }

//    return;
    std::cout << "Vz[0] (again) = " << Vz[0] << std::endl;


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
        const unsigned biggestIndex = n_a - 1;
        complex_t top = Ds[biggestIndex] * model[n_a * biggestIndex + i];
        complex_t bottom = Dz2[biggestIndex] * Vz[n_a * biggestIndex + i];
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
        for (unsigned i = 0; i < n_a; i++)
        {
            const unsigned index = i + j * n_a;
            visCorrected[index] = gains[i] * visCorrected[index] * std::conj(gains[j]);
        }
    }
}

/**
 * @details
 * Sets the diagonals of a square matrix to zero.
 * TODO: Probably not even used now! (DELETE?)
 */
void ZenithCalibrater::_zeroDiagonals(complex_t* matrix, const unsigned size)
{
    for (unsigned i = 0; i < size; i++) {
        matrix[i * size + i] = 0.0;
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
    _vis = static_cast<VisibilityData*>(data["VisibilityData"]);
    _modelVis = static_cast<ModelVisibilityData*>(data["ModelVisibilityData"]);
    _correctedVis = static_cast<CorrectedVisibilityData*>(data["CorrectedVisibilityData"]);

    if (!_vis)
        throw QString("ZenithCalibrater: VisibilityData blob missing.");
    if (!_modelVis)
        throw QString("ZenithCalibrater: ModelVisibilityData blob missing.");
    if (!_correctedVis)
        throw QString("ZenithCalibrater: CorrectedVisibilityData blob missing.");

    if (_vis->nEntries() == 0)
        throw QString("ZenithCalibrater: Visibility data empty.");

    if (_channels.size() == 0)
            throw QString("ZenithCalibrater: No channels selected.");

    // Resize corrected visibilities.
    _correctedVis->resize(_vis->nAntennas(), _vis->nChannels(),
            _vis->nPolarisations());
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
