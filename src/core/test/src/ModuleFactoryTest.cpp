#include "ModuleFactoryTest.h"
#include "ModuleFactory.h"
#include "modules/AbstractModule.h"
#include "utility/Config.h"
#include "utility/memCheck.h"

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
        ModuleFactory factory(&emptyConfig);
        AbstractModule* module = 0;
        CPPUNIT_ASSERT_NO_THROW(module = factory.createModule("EmptyModule", "EmptyPipeline"));
        CPPUNIT_ASSERT(module != 0);
    }

    // Use Case:
    // Ask for a module with a configuration
    {
        Config config;
        ModuleFactory factory(&config);
        Config::TreeAddress_t address = factory.configRoot();
        address.append(Config::NodeId_t("module", "EmptyModule"));
        config.set(address);

        AbstractModule* module = 0;
        CPPUNIT_ASSERT_NO_THROW(module = factory.createModule("EmptyModule", "EmptyPipeline"));
        CPPUNIT_ASSERT(module != 0);
    }
}

} // namespace pelican
