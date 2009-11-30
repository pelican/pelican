#include "ModuleFactoryTest.h"
#include "ModuleFactory.h"
#include "modules/AbstractModule.h"
#include "utility/Config.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ModuleFactoryTest );
// class ModuleFactoryTest 
ModuleFactoryTest::ModuleFactoryTest()
    : CppUnit::TestFixture()
{
}

ModuleFactoryTest::~ModuleFactoryTest()
{
}

void ModuleFactoryTest::setUp()
{
}

void ModuleFactoryTest::tearDown()
{
}

void ModuleFactoryTest::test_createTestModule()
{
    // Use Case:
    // Ask for a module without any configuration
    // Expect to be given a basic object
    {
        Config emptyConfig;
        ModuleFactory m(&emptyConfig);
        AbstractModule* module = 0;
        CPPUNIT_ASSERT_NO_THROW( module = m.createModule("Test") );
        CPPUNIT_ASSERT(module != 0);
    }

    // Use Case:
    // Ask for a module with a configuration
    {
        Config testconfig;
        ModuleFactory m(&testconfig);
        QStringList moduleconfig = m.moduleConfigTree();
        testconfig.setConfigurationOption(moduleconfig, "testvalue");

        AbstractModule* module = 0;
        CPPUNIT_ASSERT_NO_THROW( module = m.createModule("Test") );
        CPPUNIT_ASSERT(module != 0);
    }
}

} // namespace pelican
