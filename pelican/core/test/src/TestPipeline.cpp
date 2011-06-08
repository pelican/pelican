#include "pelican/core/test/TestPipeline.h"
#include "pelican/core/test/EmptyModule.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "pelican/utility/memCheck.h"

namespace pelican {
namespace test {

/**
 * @details
 * Default TestPipeline constructor.
 */
TestPipeline::TestPipeline(int iterations)
    : AbstractPipeline(),  _deactivateStop(false)
{
    reset();
    _iterations = iterations;
}

/**
 * @details
 * Overloaded TestPipeline constructor.
 *
 * @param[in] requirements The data requirements of the pipeline.
 */
TestPipeline::TestPipeline(const DataRequirements& requirements, int iterations)
    : AbstractPipeline(), _deactivateStop(false)
{
    reset();
    _requiredDataRemote = requirements;
    _iterations = iterations;
}

/**
 * @details
 * TestPipeline destructor.
 */
TestPipeline::~TestPipeline()
{
}

/**
 * @details
 * Pipeline initialisation method (overloaded virtual).
 * Creates the modules in the pipeline.
 */
void TestPipeline::init()
{
}

/**
 * @details
 * Public reset method.
 */
void TestPipeline::reset()
{
    _counter = 0;
    _matchedCounter = 0;
}

/**
 * @details
 * call deactivate rather than stop
 */
void TestPipeline::setDeactivation(bool d)
{
    _deactivateStop = d;
}

bool TestPipeline::deactivation() const {
    return _deactivateStop;
}

/**
 * @details
 * Pipeline run method (overridden virtual).
 * Defines a single iteration of the pipeline.
 */
void TestPipeline::run(QHash<QString, DataBlob*>& dataHash)
{
    // Print message.
    //cout << "Running TestPipeline, iteration " << _counter << endl;

    // Check the data is correct.
    if (_requiredDataRemote == dataHash)
        ++_matchedCounter;

    // Increment counter and test for completion.
    if (++_counter >= _iterations)
    {
        if( _deactivateStop ) {
            deactivate();
        }
        else {
            stop(); // Stop the pipeline driver.
        }
    }
}

} // namespace test
} // namespace pelican
