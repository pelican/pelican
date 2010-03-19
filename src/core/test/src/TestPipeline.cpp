#include "core/test/TestPipeline.h"
#include "modules/EmptyModule.h"
#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Default TestPipeline constructor.
 */
TestPipeline::TestPipeline()
    : AbstractPipeline()
{
    _setDefaults();
}

/**
 * @details
 * Overloaded TestPipeline constructor.
 *
 * @param[in] requirements The data requirements of the pipeline.
 */
TestPipeline::TestPipeline(const DataRequirements& requirements)
    : AbstractPipeline()
{
    _setDefaults();
    _requiredDataRemote = requirements;
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
 * Pipeline run method (overridden virtual).
 * Defines a single iteration of the pipeline.
 */
void TestPipeline::run(QHash<QString, DataBlob*>& dataHash)
{
    // Print message.
    std::cout << "Running TestPipeline, iteration " << _counter << std::endl;

    // Check the data is correct.
    if (_requiredDataRemote == dataHash)
        ++_matchedCounter;

    // Increment counter and test for completion.
    if (++_counter >= _iterations)
        stop(); // Stop the pipeline driver.
}

/**
 * @details
 * Private method to set defaults. Called by constructors.
 */
void TestPipeline::_setDefaults()
{
    _iterations = 10; // by default
    reset();
}

} // namespace pelican
