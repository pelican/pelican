#include "modules/ZenithModelVisibilities.h"

#include "data/DataBlob.h"
#include "data/ModelVisibilityData.h"
#include "data/AntennaPositions.h"
#include "utility/constants.h"
#include "data/Source.h"
#include <QHash>
#include <QString>
#include <QStringList>
#include <iostream>
#include <cmath>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
ZenithModelVisibilities::ZenithModelVisibilities(const ConfigNode& config)
    : AbstractModule(config)
{
    // Register data requirement for model visibility generator.
    addGeneratedData("ModelVisibilityData");
    addServiceData("AntennaPositions");

    // Initialise local pointers.
    _modelVis = NULL;
    _antPos = NULL;

    // Read sources from the configuration node.
    QDomNodeList sourceNodes = config.getDomElement().elementsByTagName("source");
    for (int i = 0; i < sourceNodes.size(); i++) {
        QDomElement sourceElement = sourceNodes.at(i).toElement();
        double coord1 = sourceElement.attribute("coord1", "0.0").toDouble();
        double coord2 = sourceElement.attribute("coord2", "0.0").toDouble();
        double ampPolX = sourceElement.attribute("ampPolX", "0.0").toDouble();
        double ampPolY = sourceElement.attribute("ampPolY", "0.0").toDouble();
        QString coordType = sourceElement.attribute("coordType", "J2000").toLower();
        unsigned iCoordType = Source::J2000;
        if (coordType == "azel") iCoordType = Source::AZEL;
        _sourcesPolXX.push_back(Source(coord1, coord2, ampPolX, iCoordType));
        _sourcesPolYY.push_back(Source(coord1, coord2, ampPolY, iCoordType));
    }

    // Read visibility blob dimension information from the configuration mode
    // (the number of antennas is extracted from the antenna position data)
    // Get the channels to image.
    QString chan = config.getOptionText("channels", "0");
    QStringList chanList = chan.split(",", QString::SkipEmptyParts);
    _channels.resize(chanList.size());
    for (int c = 0; c < chanList.size(); c++) {
        _channels[c] = chanList.at(c).toUInt();
    }

    _freqRefChannel = config.getOption("frequencies", "referenceChannel","0").toInt();
    _freqRef = config.getOption("frequencies", "reference", "1.0e8").toDouble();
    _freqDelta = config.getOption("frequencies", "delta", "1.0e8").toDouble();

    QString pol = config.getOption("polarisation", "value", "x").toLower();
    if (pol == "x") _polarisation = POL_X;
    else if (pol == "y") _polarisation = POL_Y;
    else if (pol == "both") _polarisation = POL_BOTH;
}

/**
 * @details
 * Module destructor.
 */
ZenithModelVisibilities::~ZenithModelVisibilities()
{
}


/**
 * @details
 */
void ZenithModelVisibilities::run(QHash<QString, DataBlob*>& data)
{
    // grab model visibilities data blob from the hash and checks its assigned
    _fetchDataBlobs(data);

    // calculate the model visibilities
    complex_t* vis = _modelVis->ptr(0, 0);
    unsigned nAnt = _antPos->nAntennas();
    unsigned channel = _channels[0];
    double frequency = _freqRef + (channel - _freqRefChannel) * _freqDelta;
//    std::cout << "frequency (model) = " << frequency << std::endl;

    if (_polarisation == POL_X || _polarisation == POL_BOTH) {
        _calculateModelVis(vis, nAnt, _antPos->xPtr(), _antPos->yPtr(),
                &_sourcesPolXX[0], _sourcesPolXX.size(), frequency);
    }
    if (_polarisation == POL_Y || _polarisation == POL_BOTH) {
        _calculateModelVis(vis, nAnt, _antPos->xPtr(), _antPos->yPtr(),
                &_sourcesPolYY[0], _sourcesPolYY.size(), frequency);
    }
}


/**
 * @details
 */
void ZenithModelVisibilities::_calculateModelVis(complex_t* vis,
        const unsigned nAnt, real_t* antX, real_t* antY, Source* sources,
        const unsigned nSources, const double frequency)
{
    double k = (math::twoPi * frequency) / phy::c;

    std::vector<double> lCoord(nSources);
    std::vector<double> mCoord(nSources);
    double* l = &lCoord[0];
    double* m = &mCoord[0];

    // (unless stored as ra/dec)
    // coord1 = az
    // coord2 = el
    for (unsigned i = 0; i < nSources; i++) {
        double az = sources[i].coord1();
        double el = sources[i].coord2();
//        std::cout << "s[" << i << "] az = " << az << " el = " << el << std::endl;
        double cosEl = cos(el * math::deg2rad);
        l[i] = -cosEl * sin(az * math::deg2rad);
        m[i] = cosEl * cos(az * math::deg2rad);
    }

    for (unsigned j = 0; j < nAnt; j++) {
        unsigned rowIndex = j * nAnt;
        double xj = antX[j];
        double yj = antY[j];
        for (unsigned i = 0; i < nAnt; i++) {
            unsigned index = rowIndex + i;
            double twoPiU = (xj - antX[i]) * k;
            double twoPiV = (yj - antY[i]) * k;
            for (unsigned s = 0; s < nSources; s++) {
                double arg = twoPiU * l[s] + twoPiV * m[s];
                complex_t weight = complex_t(cos(arg), sin(arg));
                vis[index] += sources[s].amp() * weight;
            }
        }
    }

//    std::vector<complex_t> antSignal(nAnt);
//    complex_t* signal = &antSignal[0];
//    for (unsigned i = 0; i < nAnt; i++) {
//        double argx = antX[i] * k;
//        double argy = antY[i] * k;
//        for (unsigned s = 0; s < nSources; s++) {
//            double arg = argx * l[s] + argy * m[s];
//            complex_t weight = complex_t(cos(arg), sin(arg));
//            signal[i] += sqrt(sources[s].amp()) * weight;
//        }
//    }
//
//
//    for (unsigned j = 0; j < nAnt; j++) {
//        unsigned rowIndex = j * nAnt;
//        complex_t sj = signal[j];
//        for (unsigned i = 0; i < nAnt; i++) {
//            vis[rowIndex + i] = sj * std::conj(signal[i]);
//        }
//    }

}


/**
 * @details
 */
void ZenithModelVisibilities::_fetchDataBlobs(QHash<QString, DataBlob*>& data)
{
    // grab model visibilities data blob from the hash and checks its assigned
    _modelVis = static_cast<ModelVisibilityData*>(data["ModelVisibilityData"]);
    _antPos = static_cast<AntennaPositions*>(data["AntennaPositions"]);

    if (!_modelVis)
        throw QString("ZenithModelVisibilities: ModelVisibilityData blob missing.");
    if (!_antPos)
        throw QString("ZenithModelVisibilities: AntennaPositions blob missing.");

    // Grab the number of antennas from the antenna position data
    unsigned nAnt = _antPos->nAntennas();
    unsigned nPol = _polarisation == POL_BOTH ? 2 : 1;
    unsigned nChannels = _channels.size();

    if (nAnt == 0)
        throw QString("ZenithModelVisibilities: No antennas found");
    if (nChannels == 0)
        throw QString("ZenithModelVisibilities: No channels selected");
    if (nPol == 0)
        throw QString("ZenithModelVisibilities: No polarisations selected");

    // resize the model visibilities
    _modelVis->resize(nAnt, nChannels, nPol);
}

} // namespace pelican
