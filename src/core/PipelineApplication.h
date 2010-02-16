#ifndef PIPELINEAPPLICATION_H
#define PIPELINEAPPLICATION_H

#include <QString>

/**
 * @file PipelineApplication.h
 */

namespace pelican {

class AbstractPipeline;
class AdapterFactory;
class Config;
class DataBlobFactory;
class ModuleFactory;
class PipelineDriver;

/**
 * @class PipelineApplication
 *  
 * @brief
 * This sets up and configures the pipeline driver.
 * 
 * @details
 * The PipelineApplication is the user-facing class which reads the
 * command-line arguments and creates:
 * - the configuration object;
 * - the adapter factory;
 * - the data blob factory;
 * - the module factory;
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
 *
 */
class PipelineApplication
{
        friend class PipelineApplicationTest;

    private:
        /// Pointer to the application's configuration object.
        Config *_config;

        /// Pointer to adapter factory.
        AdapterFactory* _adapterFactory;

        /// Pointer to the application's data blob factory.
        DataBlobFactory *_dataBlobFactory;

        /// Pointer to the application's module factory.
        ModuleFactory *_moduleFactory;

        /// The pipeline driver object.
        PipelineDriver* _driver;

    public:
        /// Constructor.
        PipelineApplication(int argc, char** argv);

        /// Destructor.
        ~PipelineApplication();

        /// Return a pointer to the application configuration object.
        Config* config() {return _config;}

        /// Register a pipeline with the pipeline driver.
        void registerPipeline(AbstractPipeline *pipeline);

        /// Sets the data client.
        void setDataClient(QString name);

        /// Starts the pipeline driver.
        void start();

    private:
        /// Creates a configuration object based on the command line arguments.
        bool _createConfig(int argc, char** argv);
};

} // namespace pelican

#endif // PIPELINEAPPLICATION_H 
