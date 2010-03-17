#ifndef TESTPIPELINE_H
#define TESTPIPELINE_H

#include "core/AbstractPipeline.h"
#include "data/DataRequirements.h"

/**
 * @file TestPipeline.h
 */

namespace pelican {

/**
 * @class TestPipeline
 *  
 * @brief
 * This class tests methods of AbstractPipeline by inheriting it.
 * 
 * @details
 * The TestPipeline class is used as the basis of unit testing the
 * AbstractPipeline class, of which this is a friend.
 */
class TestPipeline : public AbstractPipeline
{
    private:
        int _iterations;
        int _counter;
        int _matchedCounter;

    public:
        /// Default constructor.
        TestPipeline();

        /// Overloaded constructor.
        TestPipeline(const DataRequirements& requirements);

        /// Destructor.
        ~TestPipeline();

        /// Reads counter.
        int count() const {return _counter;}

        /// Initialises the pipeline.
        void init();

        /// Reads counter used to check that the data passed is the same as the expected data.
        int matchedCounter() const {return _matchedCounter;}

        /// Reset counters.
        void reset();

        /// Runs the pipeline.
        void run(QHash<QString, DataBlob*>& dataHash);

        /// Sets the number of pipeline iterations to perform.
        void setIterations(int number) {_iterations = number;}

    private:
        /// Sets the default member variables.
        void _setDefaults();
};

} // namespace pelican

#endif // TESTPIPELINE_H 
