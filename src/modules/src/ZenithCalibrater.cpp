#include "modules/ZenithCalibrater.h"
#include "data/DataBlob.h"
#include "utility/ConfigNode.h"
#include "data/VisibilityData.h"
#include "data/AntennaPositions.h"
#include "data/ModelVisibilityData.h"

#include "utility/memCheck.h"

namespace pelican {

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
 *
 * Runs the calibration module using the supplied hash of data blobs.
 */
void ZenithCalibrater::run(QHash<QString, DataBlob*>& data)
{
    // Extract and check data blobs from the hash
    _fetchDataBlobs(data);

    unsigned nAnt = _vis->nAntennas();
    unsigned nChan = _channels.size();
    unsigned nPol = _polarisation == POL_BOTH ? 2 : 1;
    unsigned nVis = nAnt * nAnt;    // number of vis per chan per pol

    // Create a temporary visibility data array used in the calibration
    // iteration.
    std::vector<complex_t> visTemp(nAnt * nAnt);

    for (unsigned c = 0; c < nChan; c++) {
        unsigned chan = _channels[c];

        for (unsigned p = 0; p < nPol; p++) {

            unsigned pol = (nPol = 1 && _polarisation == POL_X) ? 1 : 2;

            complex_t* vis = _vis->ptr(chan, pol);
            // visTemp = std::vector<complex_t> v(vis, vis + nVis * sizeof(complex_t))
            memcpy(&visTemp[0], vis, nVis * sizeof(complex_t));
            _zeroDiagonals(&visTemp[0], nAnt);

            _calibrate(&visTemp[0]);
        }
    }
}


/**
 * @details
 */
void ZenithCalibrater::_calibrate(complex_t* visTemp)
{


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
    _nEigenvalues = config.getOption("eigenvalues", "number", "10").toUInt();
    _tolerance = config.getOption("tolerance", "value", "1.0e-4").toDouble();
    _freqRefChannel = config.getOption("frequencies", "referenceChannel","0").toInt();
    _freqRef = config.getOption("frequencies", "reference", "0.0").toDouble();
    _freqDelta = config.getOption("frequencies", "delta", "195312.5").toDouble();

    QString pol = config.getOption("polarisation", "value", "x").toLower();
    if (pol == "x") _polarisation = POL_X;
    else if (pol == "y") _polarisation = POL_Y;
    else if (pol == "both") _polarisation = POL_BOTH;
}

/**
 * @details
 */

} // namespace pelican
