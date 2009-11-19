#ifndef ABSTRACTPIPELINE_H
#define ABSTRACTPIPELINE_H

#include "DataRequirements.h"

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

class DataBlob;

class AbstractPipeline
{
    public:
        AbstractPipeline(  );
        ~AbstractPipeline();

        /// Return the data requirements for the pipeline.
        const DataRequirements& dataRequired() const;

        /// Initialises the pipeline.
        virtual void init() = 0;

        /// Runs the pipeline.
        virtual void run(QHash<QString, DataBlob*>& dataHash) = 0;

    protected:
        DataRequirements _data;

    private:
};

} // namespace pelican
#endif // ABSTRACTPIPELINE_H 
