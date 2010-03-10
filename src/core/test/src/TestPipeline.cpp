#include "core/test/TestPipeline.h"
#include "modules/EmptyModule.h"
#include "utility/memCheck.h"
#include <iostream>

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
    _data = requirements;
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
    EmptyModule* testmodule = static_cast<EmptyModule*>(createModule("Empty"));
//    testmodule->setDataRequirements(_data);
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
 * Pipeline run method (overloaded virtual).
 * Defines a single iteration of the pipeline.
 */
void TestPipeline::run(QHash<QString, DataBlob*>& dataHash)
{
    if (dataRequired() == dataHash)
        ++_matchedCounter;

    /* Increment counter and test for completion */
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
