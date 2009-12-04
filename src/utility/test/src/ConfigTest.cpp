#include "ConfigTest.h"
#include "Config.h"
#include <QCoreApplication>
#include <QTextStream>
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
 * Test for empty configuration file name string.
 */
void ConfigTest::test_emptyFileName()
{
    // Use case
    // Constructs a configuration with a blank file name string.
    // Expected not to throw an exception.
    CPPUNIT_ASSERT_NO_THROW(Config config(""));
}


/**
 * @details
 * Test for configuration file that doesn't exist.
 */
void ConfigTest::test_fileDoesntExist()
{
    // Use case
    // Constructs a configuration file with a filename that doesn't exist
    // Expect to throw an exception
     CPPUNIT_ASSERT_THROW(Config config("wibble"), QString);
}


/**
 * @details
 * Test for invalid configuration file doc type.
 */
void ConfigTest::test_invalidDocType()
{
    // Use case
    // Constructs a configuration file with an invalid doc type
    // Expect to throw an exception
}


/**
 * @details
 * Test for setting configuration options
 */
void ConfigTest::test_setConfiguration()
{
    Config config;

    // Use case
    // Sets a configuration.
    // Expect that it exists.
    {
        Config::TreeAddress_t address;
        address << Config::NodeId_t("modules", "");
        address << Config::NodeId_t("module", "test1");
        QDomElement e = config.set(address);
        CPPUNIT_ASSERT(e.parentNode().toElement().tagName() == "modules");
        CPPUNIT_ASSERT(e.tagName() == "module");
        CPPUNIT_ASSERT(e.attribute("name") == "test1");
    }

    /// Use case
    /// Sets a configuration by address and attribute
    /// Expects that it exists
    {
        Config::TreeAddress_t address;
        address << Config::NodeId_t("servers", "");
        address << Config::NodeId_t("server", "test");
        QString ip = "123.456.789.10";
        config.setAttribute(address, "ip", ip);
        CPPUNIT_ASSERT(config.getAttribute(address, "ip") == ip);
    }

}


/**
 * @details
 * Test for setting configuration options
 */
void ConfigTest::test_getConfiguration()
{
    Config config;

    // Use case
    // Ask for an address that doesn't exist.
    // Expect that a null element is returned.
    {
        Config::TreeAddress_t address;
        address << Config::NodeId_t("wibble", "wobble");
        QDomElement e = config.get(address);
        CPPUNIT_ASSERT(e.isNull());
    }

    // Use case
    // Ask for an attribute that doesn't exist (both address and key).
    // Expect that an empty string is returned
    {
        Config::TreeAddress_t address;
        address << Config::NodeId_t("wibble", "wobble");
        QString a = config.getAttribute(address, "flop");
        CPPUNIT_ASSERT(a.isEmpty());
    }

    // Use case
    // Ask for an attribute with a key that doesn't exist at a valid address.
    // Expect that an empty string is returned
    {
        Config::TreeAddress_t address;
        address << Config::NodeId_t("wibble", "wobble");
        config.setAttribute(address, "flip", "true");
        QString a = config.getAttribute(address, "flop");
        CPPUNIT_ASSERT(a.isEmpty());
    }

}

} // namespace pelican
