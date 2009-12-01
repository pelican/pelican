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
QDomElement& Config::getConfiguration(
        const QList< QPair<QString, QString> > &address) const
{
    QDomElement element;
    std::cout << "- Get config.\n";

    for (int i = 0; i < address.size(); i++) {

        QString tag  = address.at(i).first;
        QString name = address.at(i).second;

        /* Populate a node list with elements specified tag name */
        QDomNodeList nodes = _document.elementsByTagName(tag);

        /* no node exists of the specified tag */
        if (nodes.isEmpty()) {
            throw QString("No nodes found\n");
        }

        for (int n = 0; n < nodes.size(); n++) {
            QDomElement e = nodes.at(i).toElement();
        }
    }
    return element;
}


/**
 * @details
 *
 * @param[in]   address The address of the configuration node element to be set.
 *
 * @return
 */
QDomElement& Config::setConfiguration(
        const QList< QPair<QString, QString> > &address
){
    std::cout << std::endl;
    for (int i = 0; i < address.size(); i++) {

        QDomNodeList nodes = _document.elementsByTagName(address.at(i).first);

        /* no node exists of the specified tag */
        if (nodes.isEmpty()) {
            std::cout << "No nodes of tag " << address.at(i).first.toStdString()
                      << " exist. Creating...\n";
            _document.createElement(address.at(i).first);
        }
        /* find if the tag already exists with the name */
        else {
            std::cout << "Searching for a tag " << address.at(i).first.toStdString()
                      << " with name " << address.at(i).second.toStdString()
                      << "\n";
            for (int n = 0; n < nodes.size(); n++) {

            }
        }
//        std::cout << address.at(i).first.toStdString()
//                  << " "
//                  << address.at(i).second.toStdString()
//                  << std::endl;
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
    if (!_document.setContent(&file, true, &error, &line, &column)) {
        QString errorMsg = "Parse error (" + QString::number(line)
        + QString::number(column) + "): " + error;
        throw errorMsg;
    }

    if (_document.doctype().name() != "pelican") {
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
    _servers = _document.elementsByTagName("server");
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
    _modules = _document.elementsByTagName("module");
    for (int i = 0; i < _modules.size(); i++) {
        QDomElement e = _modules.at(i).toElement();
        QString type = e.attribute("type");
        QString name = e.attribute("name");
        QString id = type + "::" + name;
        _moduleList[id] = i;
    }
}

} /* namespace pelican */
