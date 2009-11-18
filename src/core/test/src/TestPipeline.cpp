#include "TestPipeline.h"
#include "PipelineDriver.h"


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
}

void TestPipeline::run()
{
    if (++_counter > _iterations) {
        if (_driver) {
            _driver->stop();
        }
    }
}

void TestPipeline::_setDefaults()
{
    _driver = NULL;
    _counter = 0;
    _iterations = 10; // by default
}

} // namespace pelican
