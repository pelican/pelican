#ifndef PIPELINEEXAMPLE_H
#define PIPELINEEXAMPLE_H

#include "pelican/core/AbstractPipeline.h"
#include "pelican/modules/ZenithImagerDft.h"
#include "pelican/modules/ImageWriterFits.h"
#include "pelican/data/ImageData.h"

using namespace pelican;

class PipelineExample : public AbstractPipeline
{
    private:
        // Module pointers.
        ZenithImagerDft* imager;
        ImageWriterFits *fitsWriter;

        // Local data blobs.
        ImageData* imageData;

    public:
        PipelineExample() : AbstractPipeline() {}
        virtual void init();
        virtual void run(QHash<QString, DataBlob*>& remoteData);
};

#endif // PIPELINEEXAMPLE_H 
