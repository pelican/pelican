#ifndef PIPELINEAPPLICATION_H
#define PIPELINEAPPLICATION_H

#include "CoreOptions.h"

/**
 * @file PipelineApplication.h
 */

/**
 * @class PipelineApplication
 *  
 * @brief
 * 
 * @details
 * 
 */

namespace pelican {

class PipelineApplication
{
    public:
        PipelineApplication(int argc, char** argv);
        ~PipelineApplication();

    private:
        void _init(const CoreOptions& opts);
};

} // namespace pelican
#endif // PIPELINEAPPLICATION_H 
