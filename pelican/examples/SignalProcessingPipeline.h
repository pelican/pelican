#ifndef SIGNALPROCESSINGPIPELINE_H
#define SIGNALPROCESSINGPIPELINE_H

#include "pelican/core/AbstractPipeline.h"
#include "pelican/examples/SignalAmplifier.h"

using namespace pelican;

class SignalProcessingPipeline : public AbstractPipeline
{
    public:
        // Constructor.
        SignalProcessingPipeline() : AbstractPipeline() {}

        // Initialises the pipeline.
        void init();

        // Defines one iteration of the pipeline.
        void run(QHash<QString, DataBlob*>& remoteData);

    private:
        // Module pointers.
        SignalAmplifier* amplifier;

        // Local data blob pointers.
        SignalData* outputData;
};

#endif // SIGNALPROCESSINGPIPELINE_H
