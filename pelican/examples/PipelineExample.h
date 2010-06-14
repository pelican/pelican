#ifndef PIPELINEEXAMPLE_H
#define PIPELINEEXAMPLE_H

#include "pelican/core/AbstractPipeline.h"
#include "pelican/examples/ModuleExample.h"

using namespace pelican;

class PipelineExample : public AbstractPipeline
{
    private:
        // Module pointers.
        ModuleExample* adder;
        ModuleExample* multipler;

        // Local data blob pointers.
        DataBlobExample* outputData;

    public:
        PipelineExample() : AbstractPipeline() {}
        virtual void init();
        virtual void run(QHash<QString, DataBlob*>& remoteData);
};

#endif // PIPELINEEXAMPLE_H 
