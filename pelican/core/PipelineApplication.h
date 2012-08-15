#ifndef PIPELINEAPPLICATION_H
#define PIPELINEAPPLICATION_H

/**
 * @file PipelineApplication.h
 */

#include "pelican/core/AbstractAdapter.h"
#include "pelican/data/DataBlob.h"
#include "pelican/modules/AbstractModule.h"
#include "pelican/utility/FactoryConfig.h"
#include "pelican/utility/FactoryGeneric.h"
#include "pelican/utility/ConfigNode.h"

#include <QtCore/QString>

namespace pelican {

class AbstractPipeline;
class AbstractDataClient;
class Config;
class PipelineDriver;
class DataClientFactory;
class AbstractAdapterFactory;
class OutputStreamManager;
class PipelineSwitcher;

/**
 * @ingroup c_core
 *
 * @class PipelineApplication
 *
 * @brief
 * This sets up and configures the pipeline driver.
 *
 * @details
 * The PipelineApplication is the user-facing class which reads the
 * command-line arguments and creates the objects, including:
 * - the configuration object;
 * - the various factories;
 * - the pipeline driver.
 *
 * It also provides public methods to register pipelines and start them running.
 * A QCoreApplication must be created before a PipelineApplication.
 *
 * To create a new pipeline binary, instantiate a QCoreApplication and a
 * PipelineApplication object. Call the method registerPipeline() with a
 * pointer to a new pipeline to register one or more pipelines, and set the
 * data client using setDataClient(). Finally, call start() to start the
 * pipeline driver.
 *
 * It may be helpful to modify the following example:
 *
 * \code
 * #include <QCoreApplication>
 * #include "PipelineApplication.h"
 * #include "MyPipeline.h"
 *
 * int main(int argc, char* argv[])
 * {
 *     // Create the PipelineApplication object.
 *     QCoreApplication app(argc, argv);
 *     PipelineApplication pipelineApp(argc, argv);
 *
 *     // Register a pipeline.
 *     pipelineApp.registerPipeline(new MyPipeline);
 *
 *     // Set the data client.
 *     pipelineApp.setDataClient("FileDataClient");
 *
 *     // Start the pipeline driver.
 *     pipelineApp.start();
 *
 *     return 0;
 * }
 * \endcode
 */

class PipelineApplication
{
    private:
        friend class PipelineApplicationTest;

    public:
        /// Constructor.
        PipelineApplication(int argc, char** argv);
        PipelineApplication(const Config& config );

        /// convenience method to set search paths
        void setConfigurationSearchPaths( const QList<QString>& paths );

        /// Destructor.
        ~PipelineApplication();

        /// Return a pointer to the adapter factory.
        AbstractAdapterFactory* adapterFactory();

        /// Return a pointer to the client factory.
        DataClientFactory* clientFactory();

        /// Return a pointer to the output stream manager
        OutputStreamManager* outputStreamManager();

        /// Return a pointer to the application configuration object.
        inline Config* config() { return &_config; };

        /// Return a pointer to the data blob factory.
        static FactoryGeneric<DataBlob>* dataBlobFactory();

        /// Return a pointer to the module factory.
        FactoryConfig<AbstractModule>* moduleFactory() const;

        /// Register a pipeline with the pipeline driver.
        void registerPipeline(AbstractPipeline *pipeline);

        /// add a PipelineSwitcher to the driver
        void addPipelineSwitcher(const PipelineSwitcher& switcher);

        /// Sets the data client.
        void setDataClient(const QString& name);
        void setDataClient(AbstractDataClient* client);

        /// Starts the pipeline driver.
        void start();

        /// Stops the pipeline driver.
        void stop();

        /// returns true if start has been called
        //  and the execution loop is ooperating
        bool isRunning() const;

    private:
        /// initialise the object
        //  assumes _config is set
        void _init();

        /// Creates the configuration object based on the command line arguments.
        void _createConfig(int argc, char** argv);

    private:
        // The configuration Object
        Config _config;

        // The pipeline configuration base address
        Config::TreeAddress _pipelineAddress;

        // The pipeline driver object.
        PipelineDriver* _driver;

        // Base nodes to define configuration file.
        Config::TreeAddress _osmanagerBase;

        AbstractAdapterFactory* _adapterFactory;
        DataClientFactory* _clientFactory;
        FactoryConfig<AbstractModule>* _moduleFactory;

        // signal handling function
        static void exit(int sig);
        static QList<PipelineDriver*> _allDrivers;
};

} // namespace pelican

#endif // PIPELINEAPPLICATION_H
