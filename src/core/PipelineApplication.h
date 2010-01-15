#ifndef PIPELINEAPPLICATION_H
#define PIPELINEAPPLICATION_H

#include <QCoreApplication>
#include "PipelineDriver.h"

/**
 * @file PipelineApplication.h
 */

namespace pelican {

class Config;
class ModuleFactory;
class PipelineDriver;
class AbstractDataClient;

/**
 * @class PipelineApplication
 *  
 * @brief
 * This sets up and configures the pipeline driver.
 * 
 * @details
 * The PipelineApplication class initialises the pipeline driver
 * with its configuration options, parsing the command-line arguments
 * as needed.
 * This class also creates and manages the module factory.
 */
class PipelineApplication : public QCoreApplication
{
    private:
        /// Pointer to the application's configuration object.
        static Config *_config;

        /// Pointer to the application's data client.
        static AbstractDataClient *_dataClient;

        /// Pointer to the application's module factory.
        static ModuleFactory *_factory;

        PipelineDriver _driver; // to be removed from here (create in main()).

    public:
        /// Constructor.
        PipelineApplication(int argc, char** argv);

        /// Destructor.
        ~PipelineApplication();

        /// Return a pointer to the configuration object.
        static Config* config() {return _config;}

        /// Return a pointer to the data client.
        static AbstractDataClient* dataClient() {return _dataClient;}

        /// Return a pointer to the module factory.
        static ModuleFactory* moduleFactory() {return _factory;}

        /// Gets the configuration file name.
        QString getConfigFile() const;

    private:
        /// Creates a configuration object based on the command line arguments.
        void createConfig(int argc, char** argv);

        /// Creates a data client based on the configuration.
        void createDataClient(const Config* config);

        /// Creates the module factory based on the configuration.
        void createModuleFactory(const Config* config);
};

} // namespace pelican
#endif // PIPELINEAPPLICATION_H 
