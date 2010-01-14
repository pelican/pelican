#ifndef PIPELINEAPPLICATION_H
#define PIPELINEAPPLICATION_H

#include <QCoreApplication>
#include "PipelineDriver.h"

/**
 * @file PipelineApplication.h
 */

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

namespace pelican {

class Config;
class ModuleFactory;
class PipelineDriver;
class DataClient;

class PipelineApplication : public QCoreApplication
{
    private:
        /// Pointer to the application's configuration object.
        static Config *_config;

        /// Pointer to the application's data client.
        static DataClient *_dataClient;

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
        static DataClient* dataClient() {return _dataClient;}

        /// Return a pointer to the module factory.
        static ModuleFactory* moduleFactory() {return _factory;}

        /// Gets the configuration file name.
        QString getConfigFile() const;
};

} // namespace pelican
#endif // PIPELINEAPPLICATION_H 
