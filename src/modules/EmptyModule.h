#ifndef EMPTYMODULE_H
#define EMPTYMODULE_H

#include "AbstractModule.h"
#include "utility/ConfigNode.h"

/**
 * @file EmptyModule.h
 */

namespace pelican {

/**
 * @class EmptyModule
 *  
 * @brief
 * Module which does nothing and requires no data.
 * 
 * @details
 * This is part of the module unit tests.
 */
class EmptyModule : public AbstractModule
{
    public:
        /// Module constructor.
        EmptyModule(const ConfigNode& config);

        /// Module destructor.
        ~EmptyModule();

        /// Runs the module.
        void run(QHash<QString, DataBlob*>& /*data*/) {}

        /// sets some module data requirements
        void setDataRequirements(const DataRequirements&);
};

} // namespace pelican

#endif // EMPTYMODULE_H
