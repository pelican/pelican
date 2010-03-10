#include "pipelines/TestPipelineMultipleImages.h"
#include "modules/ZenithImagerDft.h"
#include "modules/ImageWriterFits.h"
#include "modules/ZenithModelVisibilities.h"
#include "data/ImageData.h"
#include "data/ModelVisibilityData.h"
#include "data/AntennaPositions.h"
#include <iostream>

#include "utility/memCheck.h"

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
    setName("TestPipelineDirtyImage");
    _visModel = static_cast<ZenithModelVisibilities*>(createModule("ZenithModelVisibilities"));
    _imagerA = static_cast<ZenithImagerDft*>(createModule("ZenithImagerDft", "a"));
    _imagerB = static_cast<ZenithImagerDft*>(createModule("ZenithImagerDft", "b"));
    _fitsWriterA = static_cast<ImageWriterFits*>(createModule("ImageWriterFits", "a"));
    _fitsWriterB = static_cast<ImageWriterFits*>(createModule("ImageWriterFits", "b"));

    requireRemoteData("AntennaPositions");

    _modelVis = new ModelVisibilityData;
    _imageA = new ImageData;
    _imageB = new ImageData;
}


/**
 * @details
 * Runs the pipeline iteration.
 */
void TestPipelineMultipleImages::run(QHash<QString, DataBlob*>& remoteData)
{
    _antPos = static_cast<AntennaPositions*>(remoteData["AntennaPositions"]);

    _visModel->run(_antPos, _modelVis);

    _imagerA->run(_modelVis, _antPos, _imageA);
    _imagerB->run(_modelVis, _antPos, _imageB);

    _fitsWriterA->run(_imageA);
    _fitsWriterB->run(_imageB);

    stop();
}


} // namespace pelican
