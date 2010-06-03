#include "pelican/modules/ZenithModelVisibilities.h"

#include "pelican/modules/AstrometryFast.h"
#include "pelican/data/DataBlob.h"
#include "pelican/data/ModelVisibilityData.h"
#include "pelican/data/AntennaPositions.h"
#include "pelican/data/Source.h"
#include "pelican/utility/pelicanTimer.h"

#include <QHash>
#include <QString>
#include <QStringList>
#include <iostream>
#include <cmath>

#include "pelican/utility/memCheck.h"

namespace pelican {

PELICAN_DECLARE_MODULE(ZenithModelVisibilities)

/**
 * @details
 * Module constructor.
 */
ZenithModelVisibilities::ZenithModelVisibilities(const ConfigNode& config)
    : AbstractModule(config)
{
    // Create astrometry module.
    _astrometry = new AstrometryFast;

    // Extract the configuration.
    _getConfiguration(config);
}

/**
 * @details
 * Module destructor.
 */
ZenithModelVisibilities::~ZenithModelVisibilities()
{
    delete _astrometry;
}


/**
 * @details
 * Run method for zenith model visibility generator.
 * Calculates a set of visibilities for an ideal sky model defined in the
 * configuration.
 *
 * @param[in] data Hash of data blobs.
 *
 * @note
 * At present the amplitude for each polarisation is assumed constant with
 * frequency and assumed an ideal antenna model with antenna of unit collecting
 * area/gain.
 */
void ZenithModelVisibilities::run(AntennaPositions* antPos,
        ModelVisibilityData* modelVis)
{
    // Fetch and check the required data blobs from the data hash.
    _checkDataBlobs(antPos, modelVis);

    // Dimensions for model visibility set.
    unsigned nAnt = antPos->nAntennas();
    unsigned nChanModel = _channels.size();
    unsigned nPolModel = _polarisation == POL_BOTH ? 2 : 1;

    // Get time (UT1) and set up celestial data structure for astrometry.
    double UT1 = modelVis->timeStamp();

    // Convert to Modified Julian Date.
    UT1 -= 2400000.5;
    _astrometry->setCelestialParameters(&_celestialData, &_siteData, UT1);

    // Calculate source direction cosines.
    unsigned nSources = _sources.size();
    std::vector<double> l(nSources), m(nSources);
    _calculateDirectionCosines(nSources, &_sources[0], &l[0], &m[0]);

    // Loop over channels and polarisations generating model visibilities.
    for (unsigned c = 0; c < nChanModel; c++) {
        unsigned channel = _channels[c];
        double frequency = _freqRef + (channel - _freqRefChannel) * _freqDelta;

        for (unsigned p = 0; p < nPolModel; p++) {

            unsigned iPol = p;
            iPol = (nPolModel == 1 && _polarisation == POL_X) ? 0 : 1;
            Polarisation pol = (iPol == 0) ? POL_X : POL_Y;

            _calculateModelVis(modelVis->ptr(c, iPol), nAnt, antPos->xPtr(),
                    antPos->yPtr(), &_sources[0], nSources, frequency,
                    pol, &l[0], &m[0]);
        }
    }
}


/**
 * @details
 * Calculates the local direction cosines (l, m positions) for all the
 * sources in the model.
 *
 * @param[in]  nSources  Number of sources.
 * @param[in]  source    Array of source objects (length nSources).
 * @param[in,out] l      Preallocated array of l-positions (length nSources).
 * @param[in,out] m      Preallocated array of m-positions (length nSources).
 */
void ZenithModelVisibilities::_calculateDirectionCosines (
        const unsigned nSources, const Source* sources, double* l, double* m)
{
    // Calculate l and m values for the sources.
    for (unsigned i = 0; i < nSources; i++) {
        // Convert from ICRS equatorial if needed.
        double lon = sources[i].longitude();
        double lat = sources[i].latitude();
        double az, el;

        if (sources[i].coordType() == Source::J2000) {
            _astrometry->icrsEquatorialToObserved(&_siteData, &_celestialData,
                    lon, lat, &az, &el);
        } else {
            az = lon;
            el = lat;
        }

        // Find direction cosines.
        double cosEl = cos(el);
        l[i] = cosEl * sin(az);
        m[i] = cosEl * cos(az);
    }
}


/**
 * @details
 * Calculates model visibilities for an array of sources and a set of antenna
 * positions. This is performed by first generating a matrix of antenna signals
 * for each source and then cross correlating these to form the visibility set.
 * The same result can be achieved by a direct transform between sky position
 * and Fourier (UV) space however this method has higher computational cost than
 * the one used below.
 *
 * @param[out] vis          Model visibilities generated.
 * @param[in]  nAnt         Number of antennas.
 * @param[in]  antPosX      Array of antenna x coordinates (length nAnt).
 * @param[in]  antPoxY      Array of antenna y coordinates (length nAnt).
 * @param[in]  source       Array of source objects (length nSources).
 * @param[in]  nSources     Number of sources.
 * @param[in]  frequency    Frequency in Hz for which the model is calculated.
 * @param[in]  polarisation Enumerator describing the polarisation for which
 *                          model visibilities are calculated.
 * @param[in]  l            Array of source l-positions (length nSources).
 * @param[in]  m            Array of source m-positions (length nSources).
 */
void ZenithModelVisibilities::_calculateModelVis(complex_t* vis,
        const unsigned nAnt, const real_t* antPosX, const real_t* antPosY,
        const Source* sources, const unsigned nSources, const double frequency,
        const Polarisation polarisation,
        const double* l, const double* m)
{
    double k = (math::twoPi * frequency) / phy::c;

    // Calculate a vector of antenna signals for each source.
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

    // Cross correlate the antenna signals (per source) and accumulate.
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
 * Gets the data blobs required for the module and resize the model data blob.
 * Some sanity checking is performed to ensure that module will hopefully not
 * die too horribly should bad data or options be given to it.
 *
 * @param[in] data  Hash of data blobs.
 */
void ZenithModelVisibilities::_checkDataBlobs(AntennaPositions* antPos,
        ModelVisibilityData* modelVis)
{
    if (!antPos)
        throw QString("ZenithModelVisibilities: AntennaPositions blob missing.");

    if (!modelVis)
        throw QString("ZenithModelVisibilities: ModelVisibilityData blob missing.");

    // Dimensions of the model.
    unsigned nAnt = antPos->nAntennas();
    unsigned nChanModel = _channels.size();

    if (nAnt == 0)
        throw QString("ZenithModelVisibilities: No antennas found");

    if (nChanModel == 0)
        throw QString("ZenithModelVisibilities: No channels selected");

    // Resize the model visibilities.
    modelVis->resize(nAnt, _channels, _polarisation);

    // Copy the source list into the model visibility data blob.
    modelVis->sources() = _sources;
}


/**
 * @details
 * Extracts configuration for the module from the XML node.
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
         coord1 *= math::deg2rad;
         coord2 *= math::deg2rad;

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

     _freqRefChannel = config.getOption("frequencies", "referenceChannel","0").toInt();
     _freqRef = config.getOption("frequencies", "reference", "0.0").toDouble();
     _freqDelta = config.getOption("frequencies", "delta", "195312.5").toDouble();

     // Get the channel and polarisation selection.
     _channels = config.getUnsignedList("channels");
     _polarisation = config.getPolarisation();

     // Read astrometry site parameters and set the data structure.
     double siteLongitude = config.getOption("siteLongitude", "value", "0").toDouble();
     double siteLatitude = config.getOption("siteLatitude", "value", "0").toDouble();
     siteLongitude *= math::deg2rad;
     siteLatitude *= math::deg2rad;
     double deltaUT = config.getOption("deltaUT", "value", "0").toDouble();
     double pressure = config.getOption("pressure", "value", "1000").toDouble();
     _astrometry->setSiteParameters(&_siteData, siteLongitude, siteLatitude, deltaUT, pressure);
}

} // namespace pelican
