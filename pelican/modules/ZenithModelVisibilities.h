#ifndef ZENITHMODELVISIBILITIES_H
#define ZENITHMODELVISIBILITIES_H

#include "pelican/modules/AbstractModule.h"
#include "pelican/data/SiteData.h"
#include "pelican/data/CelestialData.h"
#include "pelican/data/ModelVisibilityData.h"
#include "pelican/utility/constants.h"
#include <vector>

/**
 * @file ZenithModelVisibilities.h
 */

namespace pelican {

class AbstractAstrometry;
class ConfigNode;
class DataBlob;
class AntennaPositions;
class Source;

/**
 * @class ZenithModelVisibilities
 *
 * @brief
 * Provides facilities to model a visibility set for whole-sky views.
 *
 * @details
 *
 */

class ZenithModelVisibilities : public AbstractModule
{
    public:
        /// Module constructor.
        ZenithModelVisibilities(const ConfigNode& config);

        /// Module destructor.
        ~ZenithModelVisibilities();

        /// Runs the module.
        void run(AntennaPositions* antPos, ModelVisibilityData* modelVis);

    private:
        /// Calculate source direction cosines.
        void _calculateDirectionCosines(const unsigned nSources,
                const Source* sources, double* l, double* m);

        /// Calculate model visibilities.
        void _calculateModelVis(complex_t* vis, const unsigned nAnt,
                const real_t* antPosX, const real_t* antPosY,
                const Source* sources, const unsigned nSources,
                const double frequency,
                const Polarisation polarisation,
                const double* l, const double* m);

        /// Extract data from the data hash.
        void _checkDataBlobs(AntennaPositions* antPos,
                ModelVisibilityData* modelVis);

        /// Extract configuration from the XML node. setting some defaults.
        void _getConfiguration(const ConfigNode& config);

    private:
        AbstractAstrometry* _astrometry;
        SiteData _siteData;
        CelestialData _celestialData;
        std::vector<Source> _sources;
        std::vector<unsigned> _channels;
        int _freqRefChannel;                ///< Frequency reference channel
        double _freqRef;                    ///< Reference frequency
        double _freqDelta;                  ///< Frequency delta
        Polarisation _polarisation;
};

} // namespace pelican

#endif // ZENITHMODELVISIBILITIES_H
