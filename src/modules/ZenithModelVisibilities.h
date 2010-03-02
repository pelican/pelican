#ifndef ZENITHMODELVISIBILITIES_H
#define ZENITHMODELVISIBILITIES_H

#include "modules/AbstractModule.h"
#include <vector>

/**
 * @file ZenithModelVisibilities.h
 */

namespace pelican {

class ConfigNode;
class DataBlob;
class ModelVisibilityData;
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
        void run(QHash<QString, DataBlob*>& data);

    private:
        enum  pol_t { POL_X, POL_Y, POL_BOTH } ;

        /// Calculate model visibilties
        void _calculateModelVis(complex_t* vis, const unsigned nAnt,
                const real_t* antPosX, const real_t* antPosY,
                const Source* sources, const unsigned nSources,
                const double frequency, const pol_t polarisation);

        /// Extract data from the data hash.
        void _fetchDataBlobs(QHash<QString, DataBlob*>& data);

        /// Extract configuration from the xml node. setting some defaults.
        void _getConfiguration(const ConfigNode& config);

    private:
        ModelVisibilityData* _modelVis;
        AntennaPositions* _antPos;
        std::vector<Source> _sources;
        std::vector<unsigned> _channels;
        int _freqRefChannel;                ///< Frequency reference channel
        double _freqRef;                    ///< Reference frequency
        double _freqDelta;                  ///< Frequency delta
        unsigned _polarisation;
};

} // namespace pelican

#endif // ZENITHMODELVISIBILITIES_H
