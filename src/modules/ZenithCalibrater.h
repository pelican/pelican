#ifndef ZENITHCALIBRATER_H
#define ZENITHCALIBRATER_H

#include "core/AbstractModule.h"

class QDomElement;

/**
 * @file ZenithCalibrater.h
 */

namespace pelican {

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
        ZenithCalibrater(const QDomElement& config);

        /// Module destructor.
        ~ZenithCalibrater();

        /// Runs the module.
        void run() {}
};

} // namespace pelican

#endif // ZENITHCALIBRATER_H 
