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
    /// Use case
    // Constructs a configuration file with an invalid doc type
    // Expect to throw an exception
}


/**
 * @details
 * Test for creating a module element within the DomDocument
 */
void ConfigTest::test_createModuleConfig()
{
    Config config;
    Config::TreeAddress_t address1;
    address1 << Config::NodeId_t("modules", "")
             << Config::NodeId_t("module", "test1")
             << Config::NodeId_t("param", "a");
    QDomElement in1 = config.set(address1);
    Config::TreeAddress_t address2;
    address2 << Config::NodeId_t("modules", "")
             << Config::NodeId_t("module", "test2")
             << Config::NodeId_t("param", "a");
    QDomElement in2 = config.set(address2);

    QDomElement out1 = config.get(address1);
    QDomElement out2 = config.get(address1);

    CPPUNIT_ASSERT_ASSERTION_PASS(in1 == out1);
    CPPUNIT_ASSERT_ASSERTION_PASS(in2 == out2);
}


/**
 * @details
 * Test for creating a module element within the DomDocument
 */
void ConfigTest::test_createModuleConfig2()
{
    std::cout << std::endl;
    std::cout << QString(50, QChar('#')).toStdString() << "\n";

    Config config;
    Config::TreeAddress_t address1;
    address1 << Config::NodeId_t("modules", "")
             << Config::NodeId_t("module", "test1")
             << Config::NodeId_t("param", "a")
             << Config::NodeId_t("param", "b");
    config.set(address1);

    Config::TreeAddress_t address2;
    address2 << Config::NodeId_t("modules", "")
             << Config::NodeId_t("module", "test1")
             << Config::NodeId_t("wibble", "a")
             << Config::NodeId_t("param", "");
    config.set(address2);
    config.setAttribute(address2, "stuff", "ok");

    Config::TreeAddress_t address3;
    address3 << Config::NodeId_t("modules", "")
             << Config::NodeId_t("module", "test1")
             << Config::NodeId_t("param", "b");
    config.set(address3);

    Config::TreeAddress_t address4;
    address4 << Config::NodeId_t("servers", "")
             << Config::NodeId_t("server", "test")
             << Config::NodeId_t("ip", "");
    QDomElement e = config.set(address4);

    QDomElement myElement = e.toDocument().createElement("yey");
    e.appendChild(myElement);
    e.setAttribute("address", "123.4.56.78");

    config.summary();

    Config::TreeAddress_t address5;
    address5 << Config::NodeId_t("modules", "")
             << Config::NodeId_t("module", "test1");
    std::cout << QString(50, QChar('#')).toStdString() << "\n";

    QDomElement out = config.get(address5);

    std::cout << out.firstChild().toElement().tagName().toStdString() << "\n";
    std::cout << out.firstChild().toElement().attribute("name").toStdString() << "\n";


}


} // namespace pelican
