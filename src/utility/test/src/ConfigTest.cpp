#include "ConfigTest.h"
#include "Config.h"
#include <QCoreApplication>
#include <iostream>
#include "utility/memCheck.h"


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ConfigTest );
// class ConfigTest 
ConfigTest::ConfigTest()
    : CppUnit::TestFixture()
{
}

ConfigTest::~ConfigTest()
{
}

void ConfigTest::setUp()
{
    int ac = 0;
    app = new QCoreApplication(ac, NULL);
}

void ConfigTest::tearDown()
{
    delete app;
}

/**
 * @details
 * Test for empty configuration file string
 */
void ConfigTest::test_noConfigFile()
{
    // Use case
    // Constructs a configuration with a blank file name string.
    // Expected to throw an exception.
    CPPUNIT_ASSERT_THROW(Config config(""), QString);
}


/**
 * @details
 * Test for configuration file that doesn't exist
 */
void ConfigTest::test_emptyConfigFile()
{
    // Use case
    // Constructs a configuration file with a filename that doesn't exist
    // Expect to throw an exception
    CPPUNIT_ASSERT_THROW(Config config("wibble"), QString);
}


void ConfigTest::test_invalidConfigFile()
{
}

void ConfigTest::test_readConfigFile()
{
}

void ConfigTest::test_getModuleConfig()
{
}

void ConfigTest::test_getServerConfig()
{
}


} // namespace pelican
