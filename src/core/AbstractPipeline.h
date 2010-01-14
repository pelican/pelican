#ifndef ABSTRACTPIPELINE_H
#define ABSTRACTPIPELINE_H

#include "data/DataRequirements.h"
#include <QList>

/**
 * @file AbstractPipeline.h
 */

namespace pelican {

class AbstractModule;
class ModuleFactory;
class DataBlob;

/**
 * @class AbstractPipeline
 *  
 * @brief
 * Base class for all Pelican pipeline objects.
 * 
 * @details
 * This is the base class for all Pelican pipelines, which act as containers
 * for pipeline modules.
 * 
 * Inherit from this class and implement the init() and run() virtual methods
 * to create a new pipeline.
 */
class AbstractPipeline
{
    private:
        /// Pointer to the PipelineApplication module factory.
        ModuleFactory* _moduleFactory;

        /// List of pointers to modules in this pipeline.
        QList<AbstractModule*> _modules;

    protected:
        /// The service and stream data required by the pipeline.
        DataRequirements _data;

        /// Create a pipeline module using the module factory.
        AbstractModule* createModule(const QString& moduleName);

    public:
        /// Constructs a new abstract pipeline.
        AbstractPipeline();

        /// Destroys the abstract pipeline.
        ~AbstractPipeline();

        /// Return the data requirements for the pipeline.
        const DataRequirements& dataRequired();

        /// Initialises the pipeline by creating the required modules.
        virtual void init() {}

        /// Defines a single iteration of the pipeline (pure virtual).
        virtual void run(QHash<QString, DataBlob*>& dataHash) = 0;
};

} // namespace pelican

#endif // ABSTRACTPIPELINE_H 
