#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include "data/DataRequirements.h"

class QDomElement;

/**
 * @file AbstractModule.h
 */

namespace pelican {

/**
 * @class AbstractModule
 *  
 * @brief
 * Base class for all pipeline modules.
 *
 * @details
 * This class is the base class for all Pelican pipeline modules, which
 * provide functionality to Pelican pipelines. Each module must report
 * its set of data requirements.
 */
class AbstractModule
{
    protected:
        /// The service data and stream data required by the module.
        DataRequirements _requiredData;

    public:
        /// Constructs a new module.
        AbstractModule(const QDomElement& config);

        /// Destroys the module.
        ~AbstractModule();

        /// Returns the data required by this module.
        const DataRequirements& requiredData() const {return _requiredData;}

        /// Runs the module (pure virtual).
        virtual void run() = 0;

    private:
};

} // namespace pelican
#endif // ABSTRACTMODULE_H 
