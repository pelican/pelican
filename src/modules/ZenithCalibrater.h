#ifndef ZENITHCALIBRATER_H
#define ZENITHCALIBRATER_H

#include "AbstractModule.h"


/**
 * @file ZenithCalibrater.h
 */

namespace pelican {

class ConfigNode;

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
        void run(QHash<QString, DataBlob*>& data) {}
};

} // namespace pelican

#endif // ZENITHCALIBRATER_H 
