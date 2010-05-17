#ifndef ABSTRACTPIPELINE_H
#define ABSTRACTPIPELINE_H

#include "pelican/data/DataRequirements.h"
#include "pelican/modules/AbstractModule.h"
#include "pelican/utility/FactoryConfig.h"
#include <QString>
#include <QList>

/**
 * @file AbstractPipeline.h
 */

namespace pelican {

class AbstractModule;
class DataBlob;
class PipelineDriver;

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
 * to create a new pipeline. Set the name of the pipeline using setName(), and
 * call createModule() from init() to create the pipeline modules.
 *
 * To request remote data obtained from the client, call requestRemoteData()
 * with the type of data required. The requested data will be made available
 * via the hash supplied to run().
 *
 * For example:
 *
 * \code
 * class MyPipeline : public AbstractPipeline
 * {
 *     private:
 *         // Module pointers.
 *         ZenithImagerDft* imager;
 *         ImageWriterFits *fitsWriter;
 *
 *         // Local data blobs.
 *         ImageData* imageData;
 *
 *     public:
 *         void init()
 *         {
 *             // Set the name of the pipeline.
 *             setName("MyPipeline");
 *
 *             // Create the pipeline modules.
 *             imager = (ZenithImagerDft*) createModule("ZenithImagerDft");
 *             fitsWriter = (ImageWriterFits*) createModule("ImageWriterFits");
 *
 *             // Create the data blobs.
 *             imageData = new ImageData;
 *
 *             // Request remote data.
 *             requestRemoteData("VisibilityData");
 *             return;
 *         }
 *
 *         void run(QHash<QString, DataBlob*>& data)
 *         {
 *             // Get the remote visibility data.
 *             VisibilityData* visData = data["VisibilityData"];
 *
 *             // Run each module as required.
 *             imager->run(visData, imageData);
 *             fitsWriter->run(imageData);
 *             return;
 *         }
 * };
 * \endcode
 *
 * The run() method is called each time a new hash of data is obtained from
 * the data client, and the data hash is passed as a function argument.
 */
class AbstractPipeline
{
    private:
        friend class TestPipeline;

    private:
        /// The data required by the pipeline.
        DataRequirements _requiredDataRemote;

        /// Pointer to the PipelineApplication module factory (private, not protected).
        FactoryConfig<AbstractModule>* _moduleFactory;

        /// Pointer to the pipeline driver.
        PipelineDriver* _pipelineDriver;

        /// The name of the pipeline.
        QString _pipelineName;

        /// List of pointers to modules in this pipeline.
        QList<AbstractModule*> _modules;

    protected:
        /// Create a pipeline module using the module factory.
        AbstractModule* createModule(const QString& type,
                const QString& name = QString());

        /// Sets the name of the pipeline.
        void setName(const QString& name);

        /// Stops the pipeline driver.
        void stop();

    public:
        /// Constructs a new abstract pipeline.
        AbstractPipeline();

        /// Destroys the abstract pipeline.
        virtual ~AbstractPipeline();

        /// Initialises the pipeline by creating the required modules
        /// (pure virtual).
        /// This method creates the required modules when the pipeline is
        /// initialised, and should be implemented in a subclass.
        virtual void init() = 0;

        /// Returns the name of the pipeline.
        /// This method returns the name of the pipeline for use when finding
        /// configuration settings for the pipeline modules.
        const QString& name() const {return _pipelineName;}

        /// Requests remote data from the client.
        /// This method requests a type of remote data from the client.
        void requestRemoteData(QString type);

        /// Return the remote data requirements for the pipeline.
        const DataRequirements& requiredDataRemote() const;

        /// Defines a single iteration of the pipeline (pure virtual).
        /// This method defines what happens when the pipeline is run once,
        /// and should be implemented in a subclass. It is called each
        /// time a new hash of data is obtained from the data client. The data
        /// hash is passed as a function argument.
        ///
        /// @param[in,out] data A non-const reference to the data hash,
        ///                     which may be modified by the pipeline.
        virtual void run(QHash<QString, DataBlob*>& data) = 0;

        /// Sets the module factory.
        void setModuleFactory(FactoryConfig<AbstractModule>* factory);

        /// Sets the pipeline driver.
        void setPipelineDriver(PipelineDriver* driver);
};

} // namespace pelican

#endif // ABSTRACTPIPELINE_H 
