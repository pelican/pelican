#include "SignalProcessingPipeline.h"

// Initialises the pipeline, creating any modules and data blobs required,
// and requesting any remote data.
void SignalProcessingPipeline::init()
{
    // Create the pipeline modules and any local data blobs.
    amplifier = (SignalAmplifier*) createModule("SignalAmplifier");
    outputData = (SignalData*) createBlob("SignalData");

    // Request remote data.
    requestRemoteData("SignalData");
}


// Defines a single iteration of the pipeline.
void SignalProcessingPipeline::run(QHash<QString, DataBlob*>& remoteData)
{
    // Get pointers to the remote data blob(s) from the supplied hash.
    SignalData* inputData = (SignalData*) remoteData["SignalData"];

    // Run each module as required.
    amplifier->run(inputData, outputData);

    // TODO Output the data.
}
