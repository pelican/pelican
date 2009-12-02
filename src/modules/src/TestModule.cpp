#include "TestModule.h"
#include "utility/memCheck.h"


namespace pelican {


// class TestModule 
TestModule::TestModule(const QDomElement& config)
    : AbstractModule(config)
{
}

TestModule::~TestModule()
{
}

} // namespace pelican
