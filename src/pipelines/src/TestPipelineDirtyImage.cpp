#include "TestPipelineDirtyImage.h"
#include "modules/ZenithImagerDft.h"
#include "modules/ImageWriterFits.h"
#include "modules/ZenithModelVisibilities.h"
#include "modules/ZenithCalibrater.h"
#include <iostream>

#include "utility/memCheck.h"

namespace pelican {


/**
 * @details
 */
TestPipelineDirtyImage::TestPipelineDirtyImage()
    : AbstractPipeline()
{
}


/**
 * @details
 */
TestPipelineDirtyImage::~TestPipelineDirtyImage()
{
}


/**
 * @details
 */
void TestPipelineDirtyImage::init()
{
    setName("TestPipelineDirtyImage");
    _imager = static_cast<ZenithImagerDft*>(createModule("ZenithImagerDft"));
    _fitsWriter = static_cast<ImageWriterFits*>(createModule("ImageWriterFits"));
    _modelVis = static_cast<ZenithModelVisibilities*>(createModule("ZenithModelVisibilities"));
    _calibrate = static_cast<ZenithCalibrater*>(createModule("ZenithCalibrater"));
}


/**
 * @details
 */
void TestPipelineDirtyImage::run(QHash<QString, DataBlob*>& data)
{
    std::cout << "TestPipelineDirtyImage::run()\n";
    _modelVis->run(data);
    _calibrate->run(data);
//    _imager->run(data);
//    _fitsWriter->run(data);
    stop();
}



} // namespace pelican
