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
//    Config config("");
//    QList<QPair<QString, QString> > address;
//    address << QPair<QString, QString>("modules", "")
//            << QPair<QString, QString>("module", "test");
//    address.append(qMakePair(QString("param"), QString("a")));
//
//    QDomElement e_in = config.setConfig(address);
//
//    QDomElement e_out = config.getConfig(address);
}


/**
 * @details
 * Test for creating a module element within the DomDocument
 */
void ConfigTest::test_createModuleConfig2()
{
    std::cout << std::endl;
    std::cout << QString(50, QChar('%')).toStdString() << std::endl;

    Config config;
    Config::TreeAddress_t address1;
    address1 << Config::NodeId_t("modules", "")
             << Config::NodeId_t("module", "test1")
             << Config::NodeId_t("param", "a")
             << Config::NodeId_t("param", "b");
    config.set(address1);

    config.summary(10);

    Config::TreeAddress_t address2;
    address2 << Config::NodeId_t("modules", "")
             << Config::NodeId_t("module", "test2")
             << Config::NodeId_t("param", "a");
    config.set(address2);

    config.summary(10);

    Config::TreeAddress_t address3;
    address3 << Config::NodeId_t("modules", "")
             << Config::NodeId_t("module", "test3")
             << Config::NodeId_t("param", "a")
             << Config::NodeId_t("param", "b");
    config.set(address3);

    config.summary(10);

    if (config.exists(address1)) {
        std::cout << "Config found\n\n";
    }
    else {
        std::cout << "Config not found\n\n";
    }

//    std::cout << "Returned config for:\n";
//    std::cout << " Tag  = " << e_in.tagName().toStdString() << std::endl;
//    std::cout << " name = " << e_in.attribute(QString("name")).toStdString() << std::endl;
//    std::cout << "  parent = " << e_in.parentNode().toElement().tagName().toStdString() << std::endl;
//    std::cout << "  name = " << e_in.parentNode().toElement().attribute(QString("name")).toStdString() << std::endl;
//
//    QDomElement e;
//    e.setTagName(QString("wibble"));
//    e.setAttribute(QString("name"), QString("wobble"));
//    if (e_in.isNull()) std::cout << "Doh!\n";
//    e_in.appendChild(e);
//
//    address << QPair<QString, QString>("wibble", "wobble");
//    config.getConfiguration(address);
}


} // namespace pelican
