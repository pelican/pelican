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
class ModelVisibilities;
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
        /// Calculate model visibilties
        void _calculateModelVis(complex_t* vis, const unsigned nAnt,
                real_t* antX, real_t* antY, Source* sources,
                const unsigned nSources, const double frequency);

        /// Extract data from the data hash.
        void _fetchDataBlobs(QHash<QString, DataBlob*>& data);

    private:
        enum { POL_X, POL_Y, POL_BOTH };
        ModelVisibilities* _modelVis;
        AntennaPositions *_antPos;
        std::vector<Source> _sourcesPolXX;
        std::vector<Source> _sourcesPolYY;
        std::vector<unsigned> _channels;
        unsigned _nChannels;    ///< Total number of channels
        double _maxFrequency;   ///< Maximum frequency
        unsigned _polarisation;
};

} // namespace pelican

#endif // ZENITHMODELVISIBILITIES_H
