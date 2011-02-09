#include "reference/PipelineExample.h"
#include "reference/ModuleExample.h"
#include "reference/DataBlobExample.h"

// Initialises the pipeline, creating any modules and data blobs required,
// and requesting any remote data.
void PipelineExample::init()
{
    // Create the pipeline modules.
    adder = (ModuleExample*) createModule("ModuleExample", "adder");
    multipler = (ModuleExample*) createModule("ModuleExample", "multiplier");

    // Create local data blobs.
    outputData = (DataBlobExample*) createBlob("DataBlobExample");

    // Request remote data.
    requestRemoteData("DataBlobExample1");
    requestRemoteData("DataBlobExample2");
}


// Defines a single iteration of the pipeline.
// This performs the operation on the two vectors x and y such that
// the output is given by x * (x + y)
void PipelineExample::run(QHash<QString, DataBlob*>& remoteData)
{
    // Get pointers to the remote data blobs from the supplied hash.
    DataBlobExample1* x = (DataBlobExample1*) remoteData["DataBlobExample1"];
    DataBlobExample2* y = (DataBlobExample2*) remoteData["DataBlobExample2"];

    // Run each module as required.
    adder->run(x, y, outputData);
    multipler->run(x, outputData, outputData);
}
