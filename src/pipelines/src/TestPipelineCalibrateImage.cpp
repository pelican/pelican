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
    delete _calImage;
    delete _rawImage;
    delete _psfImage;
    delete _modelImage;
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
    _imagerModel = (ZenithImagerDft*) createModule("ZenithImagerDft", "model");
    _fitsWriter = (ImageWriterFits*) createModule("ImageWriterFits");
    _modelGen = (ZenithModelVisibilities*) createModule("ZenithModelVisibilities");
    _calibrate = (ZenithCalibrater*) createModule("ZenithCalibrater");

    // Requests for remote data to be inserted in the data hash.
    requestRemoteData("AntennaPositions");
    requestRemoteData("VisibilityData");

    _calImage = new ImageData;
    _psfImage = new ImageData;
    _rawImage = new ImageData;
    _modelImage = new ImageData;
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

    _imager->run(_rawImage, antPos, rawVis);
    _imagerModel->run(_modelImage, antPos, _modelVis);
    _imager->run(_calImage, antPos, _correctedVis);
    _imager->run(_psfImage, antPos);

    _fitsWriter->fileName() = "calibPipe-raw";
    _fitsWriter->run(_rawImage);
    _fitsWriter->fileName() = "calibPipe-model";
    _fitsWriter->run(_modelImage);
    _fitsWriter->fileName() = "calibPipe-psf";
    _fitsWriter->run(_psfImage);
    _fitsWriter->fileName() = "calibPipe-calibrated";
    _fitsWriter->run(_calImage);

    stop();
}

} // namespace pelican
