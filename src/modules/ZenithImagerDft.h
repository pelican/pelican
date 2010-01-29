#ifndef ZENITHIMAGERDFT_H
#define ZENITHIMAGERDFT_H

#include "AbstractModule.h"

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
        void run(QHash<QString, DataBlob*>& data) {}
};

} // namespace pelican

#endif // ZENITHIMAGERDFT_H
