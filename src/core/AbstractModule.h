#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include <QHash>
#include <QString>
#include "data/DataRequirements.h"
#include "data/DataBlob.h"

class QDomElement;

/**
 * @file AbstractModule.h
 */

namespace pelican {

/**
 * @class AbstractModule
 *  
 * @brief
 * Abstract base class for all pipeline modules.
 *
 * @details
 * This class is the base class for all Pelican pipeline modules, which
 * provide functionality to Pelican pipelines. Each module must report
 * its set of data requirements.
 *
 * Inherit this class and implement the run() method to create a new
 * Pelican module.
 */
class AbstractModule
{
    private:
        /// The remote service data and stream data required by the module.
        DataRequirements _reqRemote;

        /// The local service and stream data required by the module.
        DataRequirements _reqLocal;

    protected:
        /// Adds the required local service data set.
        void addLocalServiceData(const QSet<QString>& list);

        /// Adds the given local service data as a requirement.
        void addLocalServiceData(const QString& string);

        /// Adds the required local stream data set.
        void addLocalStreamData(const QSet<QString>& list);

        /// Adds the given local stream data as a requirement.
        void addLocalStreamData(const QString& string);

        /// Adds the required remote service data set.
        void addRemoteServiceData(const QSet<QString>& list);

        /// Adds the given remote service data as a requirement.
        void addRemoteServiceData(const QString& string);

        /// Adds the required remote stream data set.
        void addRemoteStreamData(const QSet<QString>& list);

        /// Adds the given remote stream data as a requirement.
        void addRemoteStreamData(const QString& string);

    public:
        /// Constructs a new module.
        AbstractModule(const QDomElement& config);

        /// Destroys the module (virtual).
        virtual ~AbstractModule();

        /// Returns all the data required by this module.
        const DataRequirements requiredData() const;

        /// Runs the module (pure virtual).
        virtual void run(QHash<QString, DataBlob*>& data) = 0;
};

} // namespace pelican

#endif // ABSTRACTMODULE_H 
