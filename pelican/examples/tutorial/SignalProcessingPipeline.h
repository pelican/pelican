#ifndef SIGNALPROCESSINGPIPELINE_H
#define SIGNALPROCESSINGPIPELINE_H

#include "pelican/core/AbstractPipeline.h"

using namespace pelican;

class SignalData;
class SignalAmplifier;

class SignalProcessingPipeline : public AbstractPipeline
{
    public:
        // Constructor.
        SignalProcessingPipeline();
        
        // Destructor
        ~SignalProcessingPipeline();

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
