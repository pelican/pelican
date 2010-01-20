#ifndef PIPELINEAPPLICATION_H
#define PIPELINEAPPLICATION_H

#include "core/PipelineDriver.h"

/**
 * @file PipelineApplication.h
 */

namespace pelican {

class Config;
class ModuleFactory;
class PipelineDriver;
class AbstractPipeline;
class AbstractDataClient;

/**
 * @class PipelineApplication
 *  
 * @brief
 * This sets up and configures the pipeline driver.
 * 
 * @details
 * The PipelineApplication is the user-facing class which reads the
 * command-line arguments and creates the configuration object, the data
 * client, the module factory and the pipeline driver. It also provides
 * public methods to register pipelines and start them running.
 *
 * A QCoreApplication must be created before a PipelineApplication.
 */
class PipelineApplication
{
        friend class PipelineApplicationTest;

    private:
        /// Pointer to the application's configuration object.
        Config *_config;

        /// Pointer to the application's data client.
        AbstractDataClient *_dataClient;

        /// Pointer to the application's module factory.
        ModuleFactory *_factory;

        /// The pipeline driver object.
        PipelineDriver _driver;

    public:
        /// Constructor.
        PipelineApplication(int argc, char** argv);

        /// Destructor.
        ~PipelineApplication();

        /// Return a pointer to the configuration object.
        Config* config() {return _config;}

        /// Return a pointer to the data client.
        AbstractDataClient* dataClient() {return _dataClient;}

        /// Return a pointer to the module factory.
        ModuleFactory* moduleFactory() {return _factory;}

        /// Gets the configuration file name.
        QString getConfigFile() const;

        /// Register a pipeline with the pipeline driver.
        void registerPipeline(AbstractPipeline *pipeline);

        /// Starts the pipeline driver.
        void start();

    private:
        /// Creates a configuration object based on the command line arguments.
        bool _createConfig(int argc, char** argv);

        /// Creates a data client based on the configuration.
        void _createDataClient(const Config* config);

        /// Creates the module factory based on the configuration.
        void _createModuleFactory(const Config* config);
};

} // namespace pelican

#endif // PIPELINEAPPLICATION_H 
