#ifndef ZENITHCALIBRATER_H
#define ZENITHCALIBRATER_H

/**
 * @file ZenithCalibrater.h
 */

#include "modules/AbstractModule.h"
#include "data/Source.h"
#include <vector>


namespace pelican {

class ConfigNode;
class VisibilityData;
class ModelVisibilityData;
class AntennaPositions;


/**
 * @class ZenithCalibrater
 *
 * @brief
 * Provides basic calibration routines for whole-sky views.
 *
 * @details
 *
 */
class ZenithCalibrater : public AbstractModule
{
    public:
        /// Module constructor.
        ZenithCalibrater(const ConfigNode& config);

        /// Module destructor.
        ~ZenithCalibrater();

        /// Runs the module.
        void run(QHash<QString, DataBlob*>& data);

    private:
        /// Grab configuration options from the config node
        void _getConfiguration(const ConfigNode& config);

    private:
        VisibilityData* _vis;
        ModelVisibilityData* _modelVis;
        AntennaPositions* _antPos;

        enum { POL_X, POL_Y, POL_BOTH };
        unsigned _nEigenvalues;
        double _tolerance;
        std::vector<Source> _sourcesPolXX;
        std::vector<Source> _sourcesPolYY;
        std::vector<unsigned> _channels;
        int _freqRefChannel;                ///< Frequency reference channel
        double _freqRef;                    ///< Reference frequency
        double _freqDelta;                  ///< Frequency delta
        unsigned _polarisation;

};

} // namespace pelican

#endif // ZENITHCALIBRATER_H
