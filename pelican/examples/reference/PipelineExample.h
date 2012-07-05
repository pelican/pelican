#ifndef PIPELINEEXAMPLE_H
#define PIPELINEEXAMPLE_H

#include "pelican/core/AbstractPipeline.h"

using namespace pelican;

class ModuleExample;
class DataBlobExample;

class PipelineExample : public AbstractPipeline
{
    public:
        PipelineExample();
        ~PipelineExample();
        virtual void init();
        virtual void run(QHash<QString, DataBlob*>& remoteData);

    private:
        // Module pointers.
        ModuleExample* adder;
        ModuleExample* multiplier;

        // Local data blob pointers.
        DataBlobExample* outputData;

};

#endif // PIPELINEEXAMPLE_H
