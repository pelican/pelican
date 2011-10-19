#ifndef TESTPIPELINE_H
#define TESTPIPELINE_H

/**
 * @file TestPipeline.h
 */

#include "pelican/core/AbstractPipeline.h"
#include "pelican/data/DataRequirements.h"

namespace pelican {
namespace test {


/**
 * @ingroup t_core
 *
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
        bool _deactivateStop;
        FactoryGeneric<DataBlob>* _blobFactory;

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

        /// Reads counter used to check that the data passed is the same as the
        /// expected data.
        int matchedCounter() const {return _matchedCounter;}

        /// return the deactivation setting
        bool deactivation() const;

        /// set the hisrory size for the specified stream
        void setHistory( const QString& stream, int size);

        /// Reset counters.
        void reset();
        void setDeactivation(bool v=true);

        /// Runs the pipeline.
        void run(QHash<QString, DataBlob*>& dataHash);
};

} // namespace test
} // namespace pelican

#endif // TESTPIPELINE_H
