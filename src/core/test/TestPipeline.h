#ifndef TESTPIPELINE_H
#define TESTPIPELINE_H


#include "AbstractPipeline.h"

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
        virtual void run();

        /// Reads counter.
        int count() {return _counter;}

        void setDriver(PipelineDriver *driver) {_driver = driver;}
        void setIterations(int number) {_iterations = number;}

    private:
        void _setDefaults();

        PipelineDriver* _driver;
        int _iterations;
        int _counter;
};

} // namespace pelican
#endif // TESTPIPELINE_H 
