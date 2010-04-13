#ifndef TESTPIPELINEMULTIPLEIMAGES_H
#define TESTPIPELINEMULTIPLEIMAGES_H

#include "pelican/core/AbstractPipeline.h"
#include <QString>
#include <QHash>

/**
 * @file TestPipelineMultipleImages.h
 */

namespace pelican {

class DataBlob;
class ZenithImagerDft;
class ImageWriterFits;
class ZenithModelVisibilities;
class ImageData;
class ModelVisibilityData;

/**
 * @class MyPipeline
 *
 * @brief
 *
 * @details
 */

class TestPipelineMultipleImages : public AbstractPipeline
{
    public:
        TestPipelineMultipleImages();
        ~TestPipelineMultipleImages();

    public:
        /// Creates the required modules.
        void init();

        /// Run method for single iteration of the pipeline.
        void run(QHash<QString, DataBlob*>& data);

    private:
        ZenithModelVisibilities* _visModel;
        ZenithImagerDft* _imagerA;
        ZenithImagerDft* _imagerB;
        ImageWriterFits* _fitsWriterA;
        ImageWriterFits* _fitsWriterB;

        ImageData* _imageA;
        ImageData* _imageB;
        ModelVisibilityData* _modelVis;

};

} // namespace pelican

#endif // TESTPIPELINEMULTIPLEIMAGES_H
