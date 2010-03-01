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
#include "utility/pelicanTimer.h"

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

    // Extract the configuration.
    _getConfiguration(config);
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
 *
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

    // TODO: loop over freq here
    {
        if (_polarisation == POL_X || _polarisation == POL_BOTH) {
            _calculateModelVis(vis, nAnt, _antPos->xPtr(), _antPos->yPtr(),
                    &_sources[0], _sources.size(), frequency, POL_X);
        }

        if (_polarisation == POL_Y || _polarisation == POL_BOTH) {
            _calculateModelVis(vis, nAnt, _antPos->xPtr(), _antPos->yPtr(),
                    &_sources[0], _sources.size(), frequency, POL_Y);
        }
    }
}


/**
 * @details
 */
void ZenithModelVisibilities::_calculateModelVis(complex_t* vis,
        const unsigned nAnt, const real_t* antPosX, const real_t* antPosY,
        const Source* sources, const unsigned nSources, const double frequency,
        const pol_t polarisation)
{
    double k = (math::twoPi * frequency) / phy::c;

    std::vector<double> lCoord(nSources);
    std::vector<double> mCoord(nSources);
    double* l = &lCoord[0];
    double* m = &mCoord[0];

    // Calculate l and m values for the sources
    for (unsigned i = 0; i < nSources; i++) {
        // TODO: convert from RA, Dec if needed
        double az = sources[i].coord1();
        double el = sources[i].coord2();
        double cosEl = cos(el * math::deg2rad);
        l[i] = cosEl * sin(az * math::deg2rad);
        m[i] = cosEl * cos(az * math::deg2rad);
    }

    // Calculate antenna signals for each source
    std::vector<complex_t> antSignal(nAnt * nSources);
    complex_t* signal = &antSignal[0];

    for (unsigned s = 0; s < nSources; s++) {
        for (unsigned i = 0; i < nAnt; i++) {
            double arg = (antPosX[i] * l[s] + antPosY[i] * m[s]) * k;
            complex_t weight = complex_t(cos(arg), sin(arg));
            double amp = (polarisation == POL_X) ?
                    sources[s].amp1() : sources[s].amp2();
            signal[s * nAnt + i] = sqrt(amp) * weight;
        }
    }

    // Cross correlate the antenna signals (per source)
    for (unsigned s = 0; s < nSources; s++) {
        for (unsigned j = 0; j < nAnt; j++) {
            for (unsigned i = 0; i < nAnt; i++) {
                complex_t signalJ = signal[s * nAnt + j];
                complex_t signalI = signal[s * nAnt + i];
                vis[j * nAnt + i] += signalJ * std::conj(signalI);
            }
        }
    }
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

    // Resize the model visibilities
    _modelVis->resize(nAnt, nChannels, nPol);

    // Insert the source list into the model visibilities
    _modelVis->sources() = _sources;
}


/**
 * @details
 */
void ZenithModelVisibilities::_getConfiguration(const ConfigNode& config)
{
    // Read sources from the configuration node.
     QDomNodeList sourceNodes = config.getDomElement().elementsByTagName("source");
     for (int i = 0; i < sourceNodes.size(); i++) {
         QDomElement sourceElement = sourceNodes.at(i).toElement();

         QString cType = sourceElement.attribute("coordType", "J2000").toLower();
         Source::coord_t coordType;
         if (cType == "j2000")
             coordType = Source::J2000;
         else if (cType == "azel")
             coordType = Source::AZEL;
         else
             throw QString("ZenithModelVisibilities: Unknown source coordinate type.");

         double coord1 = sourceElement.attribute("coord1", "0.0").toDouble();
         double coord2 = sourceElement.attribute("coord2", "0.0").toDouble();

         QString aType = sourceElement.attribute("ampType", "polxy").toLower();
         Source::amp_t ampType;
         if (aType == "polxy")
             ampType = Source::POL_XY;
         else
             throw QString("ZenithModelVisibilities: Unknown source amplitude type.");

         double ampPolX = sourceElement.attribute("amp1", "0.0").toDouble();
         double ampPolY = sourceElement.attribute("amp2", "0.0").toDouble();

         Source s(coordType, coord1, coord2, ampType, ampPolX, ampPolY);
         _sources.push_back(s);
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
     _freqRef = config.getOption("frequencies", "reference", "0.0").toDouble();
     _freqDelta = config.getOption("frequencies", "delta", "195312.5").toDouble();

     QString pol = config.getOption("polarisation", "value", "x").toLower();
     if (pol == "x") _polarisation = POL_X;
     else if (pol == "y") _polarisation = POL_Y;
     else if (pol == "both") _polarisation = POL_BOTH;
}

} // namespace pelican
