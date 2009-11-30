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
    read();
}


/**
 * @details
 * Configuration destructor.
 */
Config::~Config()
{
}


/**
 * @details
 */
QDomElement& Config::getConfiguration(const QList< QPair<QString, QString> > &address) const
{




    QDomElement element;


    //    QDomElement *config;
    // work out from the address which node to return....

    //    if (!_moduleList.contains(moduleId)) {
    //        throw QString("Requested unknown configuration");
    //    }
    //    *config = _modules.at(_moduleList[moduleId]);
    return element;
}


/**
 * @details
 *
 * @param
 *
 * @return
 */
QDomElement& Config::setConfiguration(const QList< QPair<QString, QString> > &address)
{
    for (int i = 0; i < address.size(); i++) {
        std::cout << address.at(i).first.toStdString()
                  << " "
                  << address.at(i).second.toStdString()
                  << std::endl;
    }
    QDomElement a;
    return a;
}


/**
 * @details
 */
void Config::setConfigurationAttribute(
        const QList< QPair<QString, QString> > &address,
        const QString &key,
        const QString &value
){

}


/**
 * @details
 */
void Config::read()
{
    QFile file(_fileName);

    if (_fileName.isEmpty()) return;

    if (!file.exists()) {
        QString err = "Configuration file \"" + _fileName + "\" does not exist";
        throw err;
    }

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    /* Read the XML configuration file into the QDomdocument */
    QString error; int line, column;
    if (!_domDocument.setContent(&file, true, &error, &line, &column)) {
        QString errorMsg = "Parse error (" + QString::number(line)
        + QString::number(column) + "): " + error;
        throw errorMsg;
    }

    if (_domDocument.doctype().name() != "pelican") {
        throw QString("Configuration file is not a pelican configuration document");
    }

    parseServerConfig();
    parseModuleConfig();
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
