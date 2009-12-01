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
 *
 * @param[in]   address The address of the configuration node element to be set.
 *
 * @return
 */
QDomElement& Config::setConfiguration(const TreeAddress_t &address)
{
    std::cout << "\n= Set Configuration\n";

    QDomElement parent = _document.documentElement();

    if (parent.isNull()) {
        QDomElement e = _document.createElement(QString("config"));
        _document.appendChild(e);
        parent = e;
    }

    for (int i = 0; i < address.size(); i++) {

        QString tag  = address.at(i).first;
        QString name = address.at(i).second;

        QDomNodeList nodes = parent.elementsByTagName(tag);

        /* No node exists of the specified tag - create one...*/
        if (nodes.isEmpty()) {
            QDomElement e = _document.createElement(tag);
            e.setAttribute(QString("name"), name);
            parent.appendChild(e);
            parent = e;
        }

        /* Find if the tag already exists with the name */
        else {
            std::cout << "Searching for a tag " << tag.toStdString()
                      << " with name " << name.toStdString()
                      << "\n";
            for (int n = 0; n < nodes.size(); n++) {

            }
        }
    }
    std::cout << "= Done.\n\n";

    return parent;
}


/**
 * @details
 */
QDomElement& Config::getConfiguration(const TreeAddress_t &address) const
{
    std::cout << "- Get config.\n";
    QDomElement parent = _document.documentElement();

    if (parent.isNull()) {
        std::cout << "eeek\n";
        throw QString("Empty configuration");
    }

    for (int i = 0; i < address.size(); i++) {

        QString tag  = address.at(i).first;
        QString name = address.at(i).second;

        std::cout << "looking for tag \""
                  << tag.toStdString()
                  << "\",name = \""
                  << name.toStdString()
                  << "\"\n";

        /* Populate a node list with elements specified tag name */
        QDomNodeList nodes = parent.elementsByTagName(tag);

        /* No node exists of the specified tag */
        if (nodes.isEmpty()) {
            std::cout << "No nodes found :(\n";
            throw QString("No nodes found\n");
        }

        /* Handle unique elements - ie ones with no name */
        if (name.isEmpty()) {
            if (nodes.size() != 1) {
                throw QString("Expecting unique element as no name");
            }
            else {
                parent = nodes.at(0).toElement();
                std::cout << "** Yay found unique element\n";
                continue;
            }
        }

        /* Handle named elements to find the correct element node  */
        for (int n = 0; n < nodes.size(); n++) {
            QDomElement element = nodes.at(n).toElement();
            std::cout << "tag = " << element.tagName().toStdString() << std::endl;
            if (!element.hasAttribute(QString("name"))) {
                throw QString("Expecting a name attribute to identify the tag");
            }
            else if (element.attribute(QString("name")) == name) {
                std::cout << "** Yay found named element\n";
                parent = element;
            }
        }
    }
    return parent;
}


/**
 * @details
 */
void Config::setConfigurationAttribute(
        const TreeAddress_t &address,
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
}


} /* namespace pelican */
