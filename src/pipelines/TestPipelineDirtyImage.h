#ifndef TESTPIPELINEDIRTYIMAGE_H
#define TESTPIPELINEDIRTYIMAGE_H

#include "core/AbstractPipeline.h"
#include <QString>
#include <QHash>

/**
 * @file TestPipelineDirtyImage.h
 */

namespace pelican {

class DataBlob;
class ZenithImagerDft;
class ImageWriterFits;
class ZenithModelVisibilities;
class ZenithCalibrater;

/**
 * @class TestPipelineDirtyImage
 *
 * @brief
 *
 * @details
 */

class TestPipelineDirtyImage : public AbstractPipeline
{
    public:
        TestPipelineDirtyImage();
        ~TestPipelineDirtyImage();

    public:
        /// Creates the required modules
        void init();

        /// Run method for single iteration of the pipeline
        void run(QHash<QString, DataBlob*>& data);

    private:
        ZenithImagerDft* _imager;
        ZenithModelVisibilities* _modelVis;
        ImageWriterFits* _fitsWriter;
        ZenithCalibrater* _calibrate;
};

} // namespace pelican

#endif // TESTPIPELINEDIRTYIMAGE_H
