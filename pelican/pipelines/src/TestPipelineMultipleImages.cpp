#include "pelican/pipelines/TestPipelineMultipleImages.h"

#include "pelican/modules/ZenithImagerDft.h"
#include "pelican/modules/ImageWriterFits.h"
#include "pelican/modules/ZenithModelVisibilities.h"
#include "pelican/data/ImageData.h"
#include "pelican/data/ModelVisibilityData.h"
#include "pelican/data/AntennaPositions.h"

#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 * @details
 */
TestPipelineMultipleImages::TestPipelineMultipleImages()
    : AbstractPipeline()
{
}


/**
 * @details
 */
TestPipelineMultipleImages::~TestPipelineMultipleImages()
{
}


/**
 * @details
 * Initialises the pipeline.
 */
void TestPipelineMultipleImages::init()
{
    _visModel = (ZenithModelVisibilities*) createModule("ZenithModelVisibilities");
    _imagerA = (ZenithImagerDft*) createModule("ZenithImagerDft", "a");
    _imagerB = (ZenithImagerDft*) createModule("ZenithImagerDft", "b");
    _fitsWriterA = (ImageWriterFits*) createModule("ImageWriterFits", "a");
    _fitsWriterB = (ImageWriterFits*) createModule("ImageWriterFits", "b");

    requestRemoteData("AntennaPositions");

    _modelVis = (ModelVisibilityData*) createBlob("ModelVisibilityData");
    _imageA = (ImageData*) createBlob("ImageData");
    _imageB = (ImageData*) createBlob("ImageData");
}


/**
 * @details
 * Runs the pipeline iteration.
 */
void TestPipelineMultipleImages::run(QHash<QString, DataBlob*>& remoteData)
{
    AntennaPositions* antPos = (AntennaPositions*) remoteData["AntennaPositions"];
    Q_ASSERT(antPos != NULL);

    _visModel->run(antPos, _modelVis);

    _imagerA->run(_imageA, antPos, _modelVis);
    _imagerB->run(_imageB, antPos);

    _fitsWriterA->run(_imageA);
    _fitsWriterB->run(_imageB);

    stop();
}

} // namespace pelican
