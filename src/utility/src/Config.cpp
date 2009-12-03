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
 *
 * @param[in]   address The address of the configuration node element to be set.
 *
 * @return
 */
QDomElement Config::set(const TreeAddress_t &address)
{
    QDomElement parent = _document.documentElement();

    if (parent.isNull()) {
        QDomElement e = _document.createElement(QString("config"));
        _document.appendChild(e);
        parent = e;
    }

    for (int i = 0; i < address.size(); i++) {

        QString tag  = address.at(i).first;
        QString name = address.at(i).second;

        /* Find child nodes of the specified tag type */
        QDomNodeList children = parent.childNodes();
        QList<QDomNode> nodes;
        for (int i = 0; i < children.size(); i++) {
            if (children.at(i).toElement().tagName() == tag) {
                nodes << children.at(i);
            }
        }

        /* No node exists of the specified tag - create one...*/
        if (nodes.isEmpty()) {
            _createChildNode(parent, tag, name);
            parent = parent.lastChild().toElement();
        }

        /* Nodes exist of the specified tag - do we need a new one? */
        else {

            /* Find if the tag already exists with the name */
            int iNode = -1;
            for (int n = 0; n < nodes.size(); n++) {
                QString nodeName = nodes.at(n).toElement().attribute(QString("name"));
                if (nodeName == name) {
                    iNode = n;
                }
            }

            /* No tag exists with the unique name - append one to the end */
            if (iNode == -1) {
                _createChildNode(parent, tag, name);
                parent = parent.lastChild().toElement();
            }

            /* Tag exists - move the parent pointer */
            else {
                parent = nodes.at(iNode).toElement();
            }
        }
    }

    return parent;
}


/**
 * @details
 */
const QDomElement Config::get(const TreeAddress_t &address) const
{
    QDomElement parent = _document.documentElement();

    /* Empty configuraiton return null element */
    if (parent.isNull()) {
        throw QString("Empty configuration");
    }

    for (int i = 0; i < address.size(); i++) {

        QString tag  = address.at(i).first;
        QString name = address.at(i).second;

        /* Find child nodes of the specified tag type */
        QDomNodeList children = parent.childNodes();
        QList<QDomNode> nodes;
        for (int i = 0; i < children.size(); i++) {
            if (children.at(i).toElement().tagName() == tag) {
                nodes << children.at(i);
            }
        }

        /* No node exists of the specified tag */
        if (nodes.isEmpty()) {
            throw QString("No nodes found of specified tag\n");
        }

        /* Nodes exist of the specified tag - find the right one to return */
        else {

            /* Find if the tag already exists with the name */
            int iNode = -1;
            for (int n = 0; n < nodes.size(); n++) {
                QString nodeName = nodes.at(n).toElement().attribute(QString("name"));
                if (nodeName == name) {
                    iNode = n;
                }
            }

            /* No tag exists with the name */
            if (iNode == -1) {
                throw QString("No node found with specified tag and name");
            }

            /* Tag exists - move the parent pointer */
            else {
                parent = nodes.at(iNode).toElement();
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
    QDomElement e = set(address);
    e.setAttribute(key, value);
}


/**
 * @details
 */
void Config::summary() const
{
    std::cout << std::endl;
    std::cout << QString(70, QChar('-')).toStdString() << std::endl;
    std::cout << _document.toString(4).toStdString();
    std::cout << QString(70, QChar('-')).toStdString() << std::endl;
    std::cout << std::endl;
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
//    std::cout << "Creating Child node: \"" << tag.toStdString() << "\", \""
//              << name.toStdString() <<"\"\n";

    QDomElement e = _document.createElement(tag);
    if (!name.isEmpty()) {
        e.setAttribute(QString("name"), name);
    }
    parent.appendChild(e);
}

} /* namespace pelican */
