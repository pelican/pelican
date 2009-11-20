#ifndef PIPELINEAPPLICATION_H
#define PIPELINEAPPLICATION_H

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

class ModuleFactory;
class PipelineDriver;
class Config;

class PipelineApplication
{
    public:
        /// Constructor.
        PipelineApplication(int argc, char** argv);

        /// Destructor.
        ~PipelineApplication();

        /// Gets the configuration file name.
        QString getConfigFile() const;

    private:
        Config *_config;
        ModuleFactory *_factory;
        PipelineDriver _driver;
};

} // namespace pelican
#endif // PIPELINEAPPLICATION_H 
