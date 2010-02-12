#ifndef ZENITHMODELVISIBILITIES_H
#define ZENITHMODELVISIBILITIES_H

#include "AbstractModule.h"
#include "utility/ConfigNode.h"


/**
 * @file ZenithModelVisibilities.h
 */

namespace pelican {

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
        void run(QHash<QString, DataBlob*>& data) {}
};

} // namespace pelican

#endif // ZENITHMODELVISIBILITIES_H
