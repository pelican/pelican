#ifndef TESTPIPELINE_H
#define TESTPIPELINE_H

#include "pelican/core/AbstractPipeline.h"
#include "pelican/data/DataRequirements.h"

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
        TestPipeline(int iterations = 0);

        /// Overloaded constructor.
        TestPipeline(const DataRequirements& requirements, int iterations = 0);

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
};

} // namespace pelican

#endif // TESTPIPELINE_H 
