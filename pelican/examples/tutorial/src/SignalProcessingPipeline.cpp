#include "tutorial/SignalProcessingPipeline.h"
#include "tutorial/SignalAmplifier.h"
#include "tutorial/SignalData.h"

// The constructor. It is good practice to initialise any pointer
// members to zero.
SignalProcessingPipeline::SignalProcessingPipeline()
    : AbstractPipeline(), amplifier(0), outputData(0) 
{
}

// The desttructor must clean up and created modules and
// any local DataBlob's created.
SignalProcessingPipeline::~SignalProcessingPipeline()
{
    delete amplifier;
    delete outputData;
}

// Initialises the pipeline, creating required modules and data blobs,
// and requesting remote data.
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

    // Output the input data.
    dataOutput(inputData, "pre");

    // Run each module as required.
    amplifier->run(inputData, outputData);

    // Output the processed data.
    dataOutput(outputData, "post");
}
