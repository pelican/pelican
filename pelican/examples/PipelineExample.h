#ifndef PIPELINEEXAMPLE_H
#define PIPELINEEXAMPLE_H

#include "pelican/core/AbstractPipeline.h"
#include "pelican/examples/ModuleExample.h"

using namespace pelican;

class PipelineExample : public AbstractPipeline
{
    public:
        PipelineExample() : AbstractPipeline() {}
        virtual void init();
        virtual void run(QHash<QString, DataBlob*>& remoteData);

    private:
        // Module pointers.
        ModuleExample* adder;
        ModuleExample* multipler;

        // Local data blob pointers.
        DataBlobExample* outputData;

};

#endif // PIPELINEEXAMPLE_H
