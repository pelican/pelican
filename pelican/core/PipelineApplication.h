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

#include <QtCore/QString>

namespace pelican {

class AbstractPipeline;
class Config;
class PipelineDriver;
class DataClientFactory;
class OutputStreamManager;

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

        /// Destructor.
        ~PipelineApplication();

        /// Return a pointer to the adapter factory.
        static FactoryConfig<AbstractAdapter>* adapterFactory();

        /// Return a pointer to the client factory.
        static DataClientFactory* clientFactory();

        /// Return a pointer to the output stream manager
        OutputStreamManager* outputStreamManager();

        /// Return a pointer to the application configuration object.
        static Config* config(std::string configFilename = "");

        /// Return a pointer to the data blob factory.
        static FactoryGeneric<DataBlob>* dataBlobFactory();

        /// Return a pointer to the module factory.
        static FactoryConfig<AbstractModule>* moduleFactory();

        /// Register a pipeline with the pipeline driver.
        void registerPipeline(AbstractPipeline *pipeline);

        /// Sets the data client.
        void setDataClient(const QString& name);

        /// Starts the pipeline driver.
        void start();

    private:
        /// Creates the configuration object based on the command line arguments.
        void _createConfig(int argc, char** argv);

    private:
        // The pipeline driver object.
        PipelineDriver* _driver;

        // Base nodes to define configuration file.
        Config::TreeAddress _osmanagerBase;
};

} // namespace pelican

#endif // PIPELINEAPPLICATION_H
