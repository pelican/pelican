#include "Config.h"
#include <iostream>
#include <QFile>
#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Configuration constructor.
 */
Config::Config(const QString &fileName)
{
    _fileName = fileName;

    if (checkFile()) {
        read();
    }

} /* Config() */


/**
 * @details
 * Configuration destructor.
 */
Config::~Config()
{
} /* ~Config() */


/**
 * @details
 */
bool Config::getConfiguration(QStringList address, QDomNode *config)
{
    //    if (!_moduleList.contains(moduleId)) return false;
    //    *config = _modules.at(_moduleList[moduleId]);
    return true;
}


/**
 * @details
 */
void Config::setConfigurationOption(QStringList address)
{

}

/**
 * @details
 */
bool Config::checkFile()
{
    // Check if the configuration file name is empty
    try {
        if (_fileName.isEmpty()) {
            throw QString("Empty configuration file name string.");
        }
    }
    catch (QString e) {
        std::cout << e.toStdString() << std::endl;
        throw e;
        return false;
    }

    // Check if the configuration file exists
    try {
        if (!QFile(_fileName).exists()) {
            throw QString("Configuration file does not exist.");
        }
    }
    catch (QString e) {
        std::cout << e.toStdString() << std::endl;
        throw e;
        return false;
    }

    return true;
}


/**
 * @details
 */
void Config::read()
{
    QFile file(_fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) return;

    /* Read the XML configuration file into the QDomdocument */
    try {
        QString error;
        int line, column;
        if (!_domDocument.setContent(&file, true, &error, &line, &column)) {
            QString errorMsg = "Parse error (" + QString::number(line)
            + QString::number(column) + "): " + error;
            throw errorMsg;
        }
    }
    catch (QString e) {
        std::cout << e.toStdString() << std::endl;
        throw e;
        return;
    }

    checkDocType();
    parseServerConfig();
    parseModuleConfig();
}



/**
 * @details
 */
void Config::checkDocType()
{
    try {
        if (_domDocument.doctype().name() != "pelican") {
            throw QString("Configuration file is not a pelican configuration document");
        }
    }
    catch (QString e) {
        std::cout << e.toStdString() << std::endl;
        throw e;
    }
}


/**
 * @details
 * Store a QHash lookup table for server configuration
 */
void Config::parseServerConfig()
{
    _servers = _domDocument.elementsByTagName("server");
    for (int i = 0; i < _servers.size(); i++) {
        QDomElement e = _servers.at(i).toElement();
        QString type = e.attribute("type");
        QString name = e.attribute("name");
        QString id = type + "::" + name;
        _serverList[id] = i;
    }
}


/**
 * @details
 * Store a QHash lookup table for module configuration
 */
void Config::parseModuleConfig()
{
    _modules = _domDocument.elementsByTagName("module");
    for (int i = 0; i < _modules.size(); i++) {
        QDomElement e = _modules.at(i).toElement();
        QString type = e.attribute("type");
        QString name = e.attribute("name");
        QString id = type + "::" + name;
        _moduleList[id] = i;
    }
}



} /* namespace pelican */
