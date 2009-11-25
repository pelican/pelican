#include "TestModule.h"
#include "utility/memCheck.h"


namespace pelican {


// class TestModule 
TestModule::TestModule(const QDomNode& config)
    : AbstractModule(config)
{
}

TestModule::~TestModule()
{
}

} // namespace pelican
