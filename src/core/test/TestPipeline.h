#ifndef TESTPIPELINE_H
#define TESTPIPELINE_H

#include "AbstractPipeline.h"
#include "utility/memCheck.h"

/**
 * @file TestPipeline.h
 */

/**
 * @class TestPipeline
 *  
 * @brief
 * 
 * @details
 * 
 */

namespace pelican {

class PipelineDriver;

class TestPipeline : public AbstractPipeline
{
    public:
        TestPipeline();
        TestPipeline(const DataRequirements& requirements);
        ~TestPipeline();

        /// Initialises the pipeline.
        virtual void init();

        /// Runs the pipeline.
        virtual void run(QHash<QString, DataBlob*>& dataHash);

        /// Reads counter.
        int count() {return _counter;}

        /// Reset method (reset counters).
        void reset();

        /// Reads counter used to check that the data passed is the same as the expected data.
        int matchedCounter() {return _matchedCounter;}

        void setDriver(PipelineDriver *driver) {_driver = driver;}
        void setIterations(int number) {_iterations = number;}

    private:
        void _setDefaults();

        PipelineDriver* _driver;
        int _iterations;
        int _counter;
        int _matchedCounter;
};

} // namespace pelican
#endif // TESTPIPELINE_H 
