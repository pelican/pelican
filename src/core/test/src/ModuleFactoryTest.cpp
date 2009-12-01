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
        ModuleFactory factory(&emptyConfig);
        AbstractModule* module = 0;
        CPPUNIT_ASSERT_NO_THROW( module = factory.createModule("Test") );
        CPPUNIT_ASSERT(module != 0);
    }

    // Use Case:
    // Ask for a module with a configuration
    {
        Config config;
        ModuleFactory factory(&config);
        Config::TreeAddress_t address;
        address.append(QPair<QString, QString>("modules", ""));
        address.append(QPair<QString, QString>("module", "Test"));
        config.setConfiguration(address);

        AbstractModule* module = 0;
        CPPUNIT_ASSERT_NO_THROW( module = factory.createModule("Test") );
        CPPUNIT_ASSERT(module != 0);
    }
}

} // namespace pelican
