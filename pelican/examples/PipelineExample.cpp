#include "PipelineExample.h"

// Initialises the pipeline, creating any modules and data blobs required,
// and requesting any remote data.
void PipelineExample::init()
{
    // Create the pipeline modules.
    imager = (ZenithImagerDft*) createModule("ZenithImagerDft");
    fitsWriter = (ImageWriterFits*) createModule("ImageWriterFits");

    // Create local data blobs.
    imageData = (ImageData*) createBlob("ImageData");

    // Request remote data.
    requestRemoteData("AntennaPositions");
    requestRemoteData("VisibilityData");
}

// Defines a single iteration of the pipeline.
void PipelineExample::run(QHash<QString, DataBlob*>& remoteData)
{
    // Get the remote data.
    AntennaPositions* antPos = (AntennaPositions*)
            remoteData["AntennaPositions"];
    VisibilityData* visData = (VisibilityData*) remoteData["VisibilityData"];

    // Run each module as required.
    imager->run(imageData, antPos, visData);
    fitsWriter->run(imageData);
}
