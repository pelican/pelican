#include "pelican/pipelines/TestPipelineCalibrateImage.h"

#include "pelican/modules/ZenithImagerDft.h"
#include "pelican/modules/ImageWriterFits.h"
#include "pelican/modules/ZenithModelVisibilities.h"
#include "pelican/modules/ZenithCalibrater.h"
#include "pelican/modules/ImageCombiner.h"
#include "pelican/data/AntennaPositions.h"
#include "pelican/data/ModelVisibilityData.h"
#include "pelican/data/VisibilityData.h"
#include "pelican/data/CorrectedVisibilityData.h"
#include "pelican/data/ImageData.h"

#include <iostream>

#include "pelican/utility/memCheck.h"

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
}


/**
 * @details
 */
void TestPipelineCalibrateImage::init()
{
    _imager = (ZenithImagerDft*) createModule("ZenithImagerDft");
    _imagerModel = (ZenithImagerDft*) createModule("ZenithImagerDft", "model");
    _fitsWriter = (ImageWriterFits*) createModule("ImageWriterFits");
    _modelGen = (ZenithModelVisibilities*) createModule("ZenithModelVisibilities");
    _calibrate = (ZenithCalibrater*) createModule("ZenithCalibrater");
    _imageCombiner = (ImageCombiner*) createModule("ImageCombiner");

    // Requests for remote data to be inserted in the data hash.
    requestRemoteData("AntennaPositions");
    requestRemoteData("VisibilityData");

    _calImage = (ImageData*) createBlob("ImageData");
    _psfImage = (ImageData*) createBlob("ImageData");
    _rawImage = (ImageData*) createBlob("ImageData");
    _modelImage = (ImageData*) createBlob("ImageData");
    _diffImage = (ImageData*) createBlob("ImageData");
    _modelVis = (ModelVisibilityData*) createBlob("ModelVisibilityData");
    _correctedVis = (CorrectedVisibilityData*) createBlob("CorrectedVisibilityData");
}


/**
 * @details
 */
void TestPipelineCalibrateImage::run(QHash<QString, DataBlob*>& remoteData)
{
    AntennaPositions* antPos = (AntennaPositions*) remoteData["AntennaPositions"];
    VisibilityData* rawVis = (VisibilityData*) remoteData["VisibilityData"];
    Q_ASSERT(antPos != NULL);
    Q_ASSERT(rawVis != NULL);

    _modelVis->setTimeStamp(rawVis->timeStamp());
    _modelGen->run(antPos, _modelVis);
    _calibrate->run(rawVis, _modelVis, _correctedVis);

    _imager->run(_rawImage, antPos, rawVis);

    _imagerModel->run(_modelImage, antPos, _modelVis);
    _imager->run(_calImage, antPos, _correctedVis);
    _imager->run(_psfImage, antPos);

    _imageCombiner->run(_calImage, _modelImage, _diffImage);

    _fitsWriter->fileName() = "calibPipe-raw";
    _fitsWriter->run(_rawImage);
    _fitsWriter->fileName() = "calibPipe-model";
    _fitsWriter->run(_modelImage);
    _fitsWriter->fileName() = "calibPipe-psf";
    _fitsWriter->run(_psfImage);
    _fitsWriter->fileName() = "calibPipe-calibrated";
    _fitsWriter->run(_calImage);
    _fitsWriter->fileName() = "calibPipe-diff_cal-model";
    _fitsWriter->run(_diffImage);

    stop();
}

} // namespace pelican
