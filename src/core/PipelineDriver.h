#ifndef PIPELINEDRIVER_H
#define PIPELINEDRIVER_H

/**
 * @file PipelineDriver.h
 */

/**
 * @class PipelineDriver
 *
 * @brief Pipeline driver class.
 *
 * @details
 * This class controls the data flow through the pipelines.
 * It is also responsible for the initialisation of the pipeline module
 * factories.
 */

namespace pelican {

class AbstractPipeline;
class ModuleFactory;

class PipelineDriver
{
    public:
        /// Constructs a new pipeline driver.
        PipelineDriver(int argc, char** argv);

        /// Destroys the pipeline driver.
        ~PipelineDriver();

        /// Registers the pipeline with the driver.
        void registerPipeline(AbstractPipeline *pipeline);
    
        /// Returns a reference to the module factory.
        ModuleFactory& moduleFactory() const;
    
        /// Starts the data flow through the pipelines.
        void start();
    private:
};

} // namespace pelican
#endif // PIPELINEDRIVER_H 
