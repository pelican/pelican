#ifndef MYPIPELINE_H
#define MYPIPELINE_H

#include "core/AbstractPipeline.h"
#include <QString>
#include <QHash>

/**
 * @file MyPipeline.h
 */

namespace pelican {

class DataBlob;
class VisibilityPrinter;

/**
 * @class MyPipeline
 *
 * @brief
 *
 * @details
 */

class TestPipelineVisibilityPrinter : public AbstractPipeline
{
    public:
        TestPipelineVisibilityPrinter();
        ~TestPipelineVisibilityPrinter();

    public:
        /// Creates the required modules
        void init();

        /// Run method for single iteration of the pipeline
        void run(QHash<QString, DataBlob*>& data);

    private:
        VisibilityPrinter* _visPrinter;

};

} // namespace pelican
#endif // MYPIPELINE_H
