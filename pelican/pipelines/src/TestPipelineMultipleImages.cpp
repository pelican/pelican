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
    delete _imageA;
    delete _imageB;
    delete _modelVis;
}


/**
 * @details
 * Initialises the pipeline.
 */
void TestPipelineMultipleImages::init()
{
    setName("TestPipelineMultipleImages");
    _visModel = (ZenithModelVisibilities*) createModule("ZenithModelVisibilities");
    _imagerA = (ZenithImagerDft*) createModule("ZenithImagerDft", "a");
    _imagerB = (ZenithImagerDft*) createModule("ZenithImagerDft", "b");
    _fitsWriterA = (ImageWriterFits*) createModule("ImageWriterFits", "a");
    _fitsWriterB = (ImageWriterFits*) createModule("ImageWriterFits", "b");

    requestRemoteData("AntennaPositions");

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
