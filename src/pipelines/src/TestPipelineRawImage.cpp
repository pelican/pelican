#include "TestPipelineRawImage.h"
#include "modules/ZenithImagerDft.h"
#include "modules/ImageWriterFits.h"
#include "data/AntennaPositions.h"
#include "data/VisibilityData.h"
#include "data/ImageData.h"

#include <iostream>

#include "utility/memCheck.h"

namespace pelican {


/**
 * @details
 */
TestPipelineRawImage::TestPipelineRawImage()
    : AbstractPipeline()
{
}


/**
 * @details
 */
TestPipelineRawImage::~TestPipelineRawImage()
{
    delete _image;
}


/**
 * @details
 */
void TestPipelineRawImage::init()
{
    setName("TestPipelineRawImage");
    _imager = (ZenithImagerDft*) createModule("ZenithImagerDft");
    _fitsWriter = (ImageWriterFits*) createModule("ImageWriterFits");

    // Requests for remote data to be inserted in the data hash.
    requestRemoteData("AntennaPositions");
    requestRemoteData("VisibilityData");

    _image = new ImageData;
}


/**
 * @details
 */
void TestPipelineRawImage::run(QHash<QString, DataBlob*>& remoteData)
{
    AntennaPositions* antPos = (AntennaPositions*) remoteData["AntennaPositions"];
    VisibilityData* rawVis = (VisibilityData*) remoteData["VisibilityData"];

    _imager->run(_image, antPos, rawVis);
    _fitsWriter->run(_image);

    stop();
}

} // namespace pelican
