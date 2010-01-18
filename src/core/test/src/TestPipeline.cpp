#include "TestPipeline.h"
#include "PipelineDriver.h"
#include "utility/memCheck.h"
#include <iostream>

namespace pelican {


// class TestPipeline
TestPipeline::TestPipeline()
    : AbstractPipeline()
{
    _setDefaults();
}

TestPipeline::TestPipeline(const DataRequirements& requirements)
    : AbstractPipeline()
{
    _setDefaults();
    _data = requirements;
}

TestPipeline::~TestPipeline()
{
}

void TestPipeline::init()
{
    createModule("Test");
}

void TestPipeline::run(QHash<QString, DataBlob*>& dataHash)
{
    if (dataRequired() == dataHash) {
        ++_matchedCounter;
    }

    if (++_counter >= _iterations)
        stop();
}

void TestPipeline::_setDefaults()
{
    _iterations = 10; // by default
    reset();
}

void TestPipeline::reset()
{
    _counter = 0;
    _matchedCounter = 0;
}

} // namespace pelican
