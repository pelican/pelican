#include "TestPipelineCalibrateImage.h"
#include "modules/ZenithImagerDft.h"
#include "modules/ImageWriterFits.h"
#include "modules/ZenithModelVisibilities.h"
#include "modules/ZenithCalibrater.h"
#include "data/AntennaPositions.h"
#include "data/ModelVisibilityData.h"
#include "data/VisibilityData.h"
#include "data/CorrectedVisibilityData.h"
#include "data/ImageData.h"

#include <iostream>

#include "utility/memCheck.h"

namespace pelican {


/**
 * @details
 */
TestPipelineCalibrateImage::TestPipelineCalibrateImage()
    : AbstractPipeline()
{
}


/**
 * @details
 */
TestPipelineCalibrateImage::~TestPipelineCalibrateImage()
{
    delete _image;
    delete _modelVis;
    delete _correctedVis;
}


/**
 * @details
 */
void TestPipelineCalibrateImage::init()
{
    setName("TestPipelineCalibrateImage");
    _imager = (ZenithImagerDft*) createModule("ZenithImagerDft");
    _fitsWriter = (ImageWriterFits*) createModule("ImageWriterFits");
    _modelGen = (ZenithModelVisibilities*) createModule("ZenithModelVisibilities");
    _calibrate = (ZenithCalibrater*) createModule("ZenithCalibrater");

    // Requests for remote data to be inserted in the data hash.
    requestRemoteData("AntennaPositions");
    requestRemoteData("VisibilityData");

    _image = new ImageData;
    _modelVis = new ModelVisibilityData;
    _correctedVis = new CorrectedVisibilityData;
}


/**
 * @details
 */
void TestPipelineCalibrateImage::run(QHash<QString, DataBlob*>& remoteData)
{
    AntennaPositions* antPos = (AntennaPositions*) remoteData["AntennaPositions"];
    VisibilityData* rawVis = (VisibilityData*) remoteData["VisibilityData"];

    _modelVis->setTimeStamp(rawVis->timeStamp());
    _modelGen->run(antPos, _modelVis);

    _calibrate->run(rawVis, _modelVis, _correctedVis);

    _imager->run(_image, antPos, _correctedVis);
    _fitsWriter->run(_image);

    stop();
}

} // namespace pelican
