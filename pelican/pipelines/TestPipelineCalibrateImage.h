#ifndef TESTPIPELINECALIBRATEIMAGE_H
#define TESTPIPELINECALIBRATEIMAGE_H

#include "pelican/core/AbstractPipeline.h"
#include <QString>
#include <QHash>

/**
 * @file TestPipelineCalibrateImage.h
 */

namespace pelican {

class DataBlob;
class ZenithImagerDft;
class ImageWriterFits;
class ZenithModelVisibilities;
class ZenithCalibrater;
class ImageCombiner;

class ModelVisibilityData;
class CorrectedVisibilityData;
class ImageData;

/**
 * @class TestPipelineCalibrateImage
 *
 * @brief
 *
 * @details
 */

class TestPipelineCalibrateImage : public AbstractPipeline
{
    public:
        TestPipelineCalibrateImage();
        ~TestPipelineCalibrateImage();

    public:
        /// Creates the required modules
        void init();

        /// Run method for single iteration of the pipeline
        void run(QHash<QString, DataBlob*>& data);

    private:
        ZenithImagerDft* _imager;
        ZenithImagerDft* _imagerModel;
        ZenithModelVisibilities* _modelGen;
        ImageWriterFits* _fitsWriter;
        ZenithCalibrater* _calibrate;
        ImageCombiner* _imageCombiner;

        ModelVisibilityData* _modelVis;
        CorrectedVisibilityData* _correctedVis;
        ImageData* _calImage;
        ImageData* _rawImage;
        ImageData* _psfImage;
        ImageData* _modelImage;
        ImageData* _diffImage;
};

} // namespace pelican

#endif // TESTPIPELINECALIBRATEIMAGE_H
