#ifndef PIPELINEAPPLICATION_H
#define PIPELINEAPPLICATION_H

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
 */

namespace pelican {

class PipelineApplication
{
    public:
        PipelineApplication(int argc, char** argv);
        ~PipelineApplication();
};

} // namespace pelican
#endif // PIPELINEAPPLICATION_H 
