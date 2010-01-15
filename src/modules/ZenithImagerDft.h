#ifndef ZENITHIMAGERDFT_H
#define ZENITHIMAGERDFT_H

#include "core/AbstractModule.h"

class QDomElement;

/**
 * @file ZenithImagerDft.h
 */

namespace pelican {

/**
 * @class ZenithImagerDft
 *  
 * @brief
 * Provides basic imaging routines for whole-sky views.
 * 
 * @details
 * 
 */
class ZenithImagerDft : public AbstractModule
{
    public:
        /// Module constructor.
        ZenithImagerDft(const QDomElement& config);

        /// Module destructor.
        ~ZenithImagerDft();

        /// Runs the module.
        void run() {}
};

} // namespace pelican

#endif // ZENITHIMAGERDFT_H
