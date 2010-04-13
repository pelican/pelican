#ifndef TESTPIPELINESERVER_H
#define TESTPIPELINESERVER_H

#include "pelican/core/AbstractPipeline.h"
#include <QString>
#include <QHash>

/**
 * @file TestPipelineServer.h
 */

namespace pelican {

class DataBlob;
class VisibilityPrinter;

/**
 * @class TestPipelineServer
 *
 * @brief
 *
 * @details
 */
class TestPipelineServer : public AbstractPipeline
{
    public:
        TestPipelineServer();
        ~TestPipelineServer();

    public:
        /// Creates the required modules.
        void init();

        /// Run method for single iteration of the pipeline.
        void run(QHash<QString, DataBlob*>& data);

    private:
        VisibilityPrinter* _visPrinter;
        int _counter;

};

} // namespace pelican

#endif // TESTPIPELINESERVER_H
