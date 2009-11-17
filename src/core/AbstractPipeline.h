#ifndef ABSTRACTPIPELINE_H
#define ABSTRACTPIPELINE_H

/**
 * @file AbstractPipeline.h
 */

/**
 * @class AbstractPipeline
 *  
 * @brief
 * 
 * @details
 * 
 */

namespace pelican {

class DataRequirements;

class AbstractPipeline
{
    public:
        AbstractPipeline(  );
        ~AbstractPipeline();

        /// Return the data requirements for the pipeline.
        DataRequirements& dataRequired() const;

        /// Initialises the pipeline.
        virtual void init() = 0;

        /// Runs the pipeline.
        virtual void run() = 0;
    private:
};

} // namespace pelican
#endif // ABSTRACTPIPELINE_H 
