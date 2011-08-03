#include "ModuleFactoryTest.h"
#include "pelican/modules/AbstractModule.h"
#include "pelican/utility/FactoryConfig.h"
#include "pelican/utility/Config.h"
#include <iostream>

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ModuleFactoryTest );

/**
 * @details
 * ModuleFactoryTest constructor.
 */
ModuleFactoryTest::ModuleFactoryTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details
 * ModuleFactoryTest destructor.
 */
ModuleFactoryTest::~ModuleFactoryTest()
{
}

/**
 * @details
 * Set-up routine called before each test.
 */
void ModuleFactoryTest::setUp()
{
}

/**
 * @details
 * Clean-up routine called after each test.
 */
void ModuleFactoryTest::tearDown()
{
}

/**
 * @details
 * Tries to create an EmptyModule with and without a configuration object.
 */
void ModuleFactoryTest::test_createModule_EmptyModule()
{
    // Use Case:
    // Ask for a module without any configuration
    // Expect to be given a basic object
    {
        Config emptyConfig;
        FactoryConfig<AbstractModule> factory(&emptyConfig, "", "");
        AbstractModule* module = 0;
        CPPUNIT_ASSERT_NO_THROW(module = factory.create("EmptyModule"));
        CPPUNIT_ASSERT(module != 0);
    }

    // Use Case:
    // Ask for a module with a configuration
    {
        Config config;
        QString section("pipeline"), group("modules");
        Config::TreeAddress base;
        base.append(Config::NodeId(section, ""));
        base.append(Config::NodeId(group, ""));
        Config::TreeAddress address(base);
        address.append(Config::NodeId("EmptyModule", ""));
        config.set(address);
        FactoryConfig<AbstractModule> factory(&config, section, group);

        AbstractModule* module = 0;
        CPPUNIT_ASSERT_NO_THROW(module = factory.create("EmptyModule"));
        CPPUNIT_ASSERT(module != 0);
    }
}

} // namespace pelican
