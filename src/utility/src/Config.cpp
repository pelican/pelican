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
    _read();
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
bool Config::exists(const TreeAddress_t &address) const
        {
    std::cout << "Checking address:\n";

    for (int i = 0; i < address.size(); i++) {
        QString tag  = address.at(i).first;
        QString name = address.at(i).second;
        std::cout << "[" << i << "] tag  = " << tag.toStdString() << "\n";
        std::cout << "[" << i << "] name = " << name.toStdString() << "\n";
    }


    QDomElement parent = _document.documentElement();

    if (parent.isNull()) {
        return false;
    }

    for (int i = 0; i < address.size(); i++) {

        QString tag  = address.at(i).first;
        QString name = address.at(i).second;

        /* Populate a node list with elements specified tag name */
        QDomNodeList nodes = parent.elementsByTagName(tag);

        /* No node exists of the specified tag */
        if (nodes.isEmpty()) {
            return false;
        }

        /* Handle unique elements - ie ones with no name */
        if (name.isEmpty()) {
            if (nodes.size() != 1) {
                return false;
            }
            else {
                parent = nodes.at(0).toElement();
                continue;
            }
        }

        /* Handle named elements to find the correct element node  */
        for (int n = 0; n < nodes.size(); n++) {
            QDomElement element = nodes.at(n).toElement();
            if (!element.hasAttribute(QString("name"))) {
                return false;
            }
            else if (element.attribute(QString("name")) == name) {
                return true;
            }
        }
    }
    return true;
        }


/**
 * @details
 *
 * @param[in]   address The address of the configuration node element to be set.
 *
 * @return
 */
QDomElement& Config::set(const TreeAddress_t &address)
{
    QDomElement parent = _document.documentElement();

    if (parent.isNull()) {
        QDomElement e = _document.createElement(QString("config"));
        _document.appendChild(e);
        parent = e;
    }
    else {
        parent = parent.lastChild().toElement();
    }

    for (int i = 0; i < address.size(); i++) {

        QString tag  = address.at(i).first;
        QString name = address.at(i).second;
        QDomNodeList nodes = parent.elementsByTagName(tag);

        /* No node exists of the specified tag - create one...*/
        if (nodes.isEmpty()) {
            _createChildNode(parent, tag, name);
            parent = parent.lastChild().toElement();
        }

        /* Find if the tag already exists with the name */
        bool exists = false;
        for (int n = 0; n < nodes.size(); n++) {
            QString nodeName = nodes.at(n).toElement().attribute(QString("name"));
            if (nodeName == name) {
                exists = true;
            }
        }
        if (!exists) {
            _createChildNode(parent, tag, name);
        }
    }

    return parent;
}


/**
 * @details
 */
QDomElement& Config::get(const TreeAddress_t &address) const
        {
    QDomElement parent = _document.documentElement();

    if (parent.isNull()) {
        std::cout << "Config empty\n";
        return parent;
    }

    std::cout << std::endl << QString(50, QChar('-')).toStdString() << std::endl;
    for (int i = 0; i < address.size(); i++) {

        QString tag  = address.at(i).first;
        QString name = address.at(i).second;

        std::cout << "Parent Tag = " << parent.tagName().toStdString() << "\n";
        std::cout << " Tag  = " << tag.toStdString() << "\n";
        std::cout << " Name = " << name.toStdString() << "\n";
        std::cout << std::endl;

        /* Populate a node list with elements specified tag name */
        QDomNodeList nodes = parent.elementsByTagName(tag);

        /* No node exists of the specified tag */
        if (nodes.isEmpty()) {
            std::cout << "No nodes found of tag \"" << tag.toStdString() << "\"\n";
            throw QString("No nodes found\n");
        }

        /* Handle unique elements - ie ones with no name */
        if (name.isEmpty()) {
            if (nodes.size() != 1) {
                std::cout << "Expecting unique element as no name\n";
                throw QString("Expecting unique element as no name");
            }
            else {
                parent = nodes.at(0).toElement();
                continue;
            }
        }

        /* Handle named elements to find the correct element node  */
        for (int n = 0; n < nodes.size(); n++) {
            QDomElement element = nodes.at(n).toElement();
            if (!element.hasAttribute(QString("name"))) {
                std::cout << "Expecting a name attribute to identify the tag\n";
                throw QString("Expecting a name attribute to identify the tag");
            }
            else if (element.attribute(QString("name")) == name) {
                parent = element;
            }
        }
    }
    return parent;
        }


/**
 * @details
 */
void Config::setAttribute(
        const TreeAddress_t &address,
        const QString &key,
        const QString &value
){
}


/**
 * @details
 */
void Config::summary(const unsigned int depth) const
{
    std::cout << QString(50, QChar('-')).toStdString() << std::endl;
    std::cout << "Config summary (to depth = " << depth << "):\n";
    QDomElement parent = _document.documentElement();
    _printChildren(parent, 0, depth);
    std::cout << QString(50, QChar('-')).toStdString() << std::endl;
}


/**
 * @details
 * Read a configuration from the Pelican XML file specified when the
 * configuration object is instantiated.
 */
void Config::_read()
{
    QFile file(_fileName);

    if (_fileName.isEmpty()) {
        return;
    }

    if (!file.exists()) {
        QString err = "Configuration file does not exist: " + _fileName;
        throw err;
    }

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    /* Read the XML configuration file into the QDomdocument */
    QString error;
    int line, column;
    if (!_document.setContent(&file, true, &error, &line, &column)) {
        QString err = "Parse error (" + QString::number(line)
        + QString::number(column) + "): " + error;
        throw err;
    }

    if (_document.doctype().name() != "pelican") {
        throw QString("Invalid doctype.");
    }
}


/**
 * @details
 */
void Config::_createChildNode(QDomElement &parent, const QString& tag, const QString& name)
{
//    if (parent.isNull()) {
//        return;
//    }
    std::cout << "Creating Child node: \"" << tag.toStdString() << "\", \""
              << name.toStdString() <<"\"\n";
    QDomElement e = _document.createElement(tag);
    if (!name.isEmpty()) {
        e.setAttribute(QString("name"), name);
    }
    parent.appendChild(e);
}


/**
 * @details
 * Prints Children of specified element.
 */
void Config::_printChildren(const QDomElement& element, const int depth, const int maxDepth) const
{
    QDomNodeList children = element.childNodes();
    QString indent(depth * 2, QChar(' '));
    for (int i = 0; i < children.size(); i++) {
        QDomElement c = children.at(i).toElement();
        std::cout << indent.toStdString();
        std::cout << "<" << c.tagName().toStdString();
        QString name = c.attribute(QString("name"));
        int nAttributes = c.attributes().count();
        if (!name.isEmpty()) {
            std::cout << " name=\"" << name.toStdString() << "\"";
            nAttributes--;
        }
        if (nAttributes > 0) {
            std::cout << " ...";
        }
        std::cout << ">\n";
        if (depth + 1 < maxDepth) {
            _printChildren(c, depth + 1, maxDepth);
        }
    }
}


} /* namespace pelican */
