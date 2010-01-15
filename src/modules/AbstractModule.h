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
    private:
        /// The service data and stream data required by the module.
        DataRequirements _req;

    protected:
        /// Adds the required service data set.
        void addServiceData(const QSet<QString>& list) {_req.addServiceData(list);}

        /// Adds the given service data as a requirement.
        void addServiceData(const QString& string) {_req.setServiceData(string);}

        /// Adds the required stream data set.
        void addStreamData(const QSet<QString>& list)  {_req.addStreamData(list);}

        /// Adds the given stream data as a requirement.
        void addStreamData(const QString& string) {_req.setStreamData(string);}

    public:
        /// Constructs a new module.
        AbstractModule(const QDomElement& config);

        /// Destroys the module.
        ~AbstractModule();

        /// Returns the data required by this module.
        const DataRequirements& requiredData() const {return _req;}

        /// Runs the module (pure virtual).
        virtual void run() = 0;
};

} // namespace pelican

#endif // ABSTRACTMODULE_H 
