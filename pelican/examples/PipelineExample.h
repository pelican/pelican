#ifndef PIPELINEEXAMPLE_H
#define PIPELINEEXAMPLE_H

#include "pelican/core/AbstractPipeline.h"

using namespace pelican;

class PipelineExample : public AbstractPipeline
{
    public:
        PipelineExample();
        virtual ~PipelineExample();

        virtual void init();
        virtual void run(QHash<QString, DataBlob*>& data);

    private:
};

#endif // PIPELINEEXAMPLE_H 
