#include "pelican/utility/Config.h"
#include <iostream>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <iostream>
#include <QDomDocumentType>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Configuration constructor.
 */
Config::Config(const QString& fileName)
{
    read(fileName);
}


/**
 * @details
 * Return the specified configuration node together with any extra required
 * varsets.
 */
ConfigNode Config::get(const TreeAddress &address) const
{
    // Declare the list of returned DOM elements.
    QList<QDomElement> elementList;

    // Get the specified address and put it into index 0.
    QDomElement node = _get(address);
    elementList.insert(0, node);

    // Check and get addresses for all varsets.
    QDomNodeList varsetList = node.elementsByTagName("varset");
    for (int i = 0; i < varsetList.size(); ++i) {
        QString varsetName = varsetList.at(i).toElement().attribute("name");
        Config::TreeAddress varsetAddress;
        varsetAddress.append(Config::NodeId("varsets", ""));
        varsetAddress.append(Config::NodeId("varset", varsetName));

        // Get the varset at the address.
        elementList.insert(i + 1, _get(varsetAddress));
    }

    return ConfigNode(elementList);
}


/**
 * @details
 * Set the the attribute at the specified address to the key, value.
 */
void Config::setAttribute(const TreeAddress &address, const QString &key,
        const QString &value)
{
    QDomElement e = _set(address);
    e.setAttribute(key, value);
}


/**
 * @details
 * Returns the attribute at the address with the specified key. An empty string
 * is returned if the address or key doesn't exist.
 */
QString Config::getAttribute(const TreeAddress& address, const QString& key) const
{
    QDomElement e = _get(address);
    if (e.isNull())
        return QString();
    else
        return e.attribute(key);
}


/**
 * @details
 * Sets the text node at the address.
 */
void Config::setText(const TreeAddress& address, const QString& text)
{
    QDomElement e = _set(address);
    QDomText t = _document.createTextNode(text);
    e.appendChild(t);
}


/**
 * @details
 * Gets value of the text node at the address. If multiple text nodes are
 * found at the same level they will be concatenated.
 */
QString Config::getText(const TreeAddress& address) const
{
    QDomElement e = _get(address);
    QDomNodeList children = e.childNodes();
    QString text = QString();
    if (children.isEmpty()) {
        return text;
    }
    else {
        for (int c = 0; c < children.size(); c++) {
            if (children.at(c).nodeType() == QDomNode::TextNode) {
                text += children.at(c).nodeValue();
            }
        }
    }
    return text;
}


/**
 * @details
 * Prints a summary of the configuration tree to STDOUT.
 */
void Config::summary() const
{
    std::cout << std::endl;
    std::cout << QString(70, QChar('=')).toStdString() << std::endl;
    std::cout << _document.toString(4).toStdString();
    std::cout << QString(70, QChar('=')).toStdString() << std::endl;
    std::cout << std::endl;
}


/**
 * @details
 * Save the Configuration to specified file name
 */
void Config::save(const QString& fileName) const
{
    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return;

    QTextStream out(&file);
    out << _document.toByteArray(4);
}


/**
 * @details
 * Sets the content of the configuration QDomDocument from the QStrings
 * \p pipelineConfig and \p serverConfig.
 *
 * @param[in] pipelineConfig String containing the pipeline XML configuration.
 * @param[in] serverConfig String containing server XML configuration.
 */
void Config::setFromString(const QString& pipelineConfig, const QString& serverConfig)
{
    _document.clear();
    _document = QDomDocument("pelican");

    // Read the XML configuration file into the QDomDocument.
    QString error;
    int line, column;

    QString xmlDoc =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<!DOCTYPE pelican>"
            "<configuration version=\"1.0\">"
            "<pipeline>"
            "" + pipelineConfig + ""
            "</pipeline>"
            "<server>"
            "" + serverConfig + ""
            "</server>"
            "</configuration>";

    if (!_document.setContent(xmlDoc, true, &error, &line, &column)) {
        throw QString("Config::setFromString(): Error parsing XML "
                "(Line: %1 Col: %2): %3.").arg(line).arg(column).arg(error);
    }
}


/**
 * @details
 * Sets and creates the specified address in the configuration document.
 *
 * @param[in] address The address of the configuration node element to be set.
 *
 * @return QDomElement at the address specified in the argument.
 */
QDomElement Config::_set(const TreeAddress &address)
{
    QDomElement parent = _document.documentElement();

    if (parent.isNull()) {
        QDomElement e = _document.createElement(QString("config"));
        _document.appendChild(e);
        parent = e;
    }

    for (int a = 0; a < address.size(); a++) {

        QString tag  = address.at(a).first;
        QString name = address.at(a).second;

        // Find child nodes of the specified tag type.
        QDomNodeList children = parent.childNodes();
        QList<QDomNode> nodes;
        for (int i = 0; i < children.size(); i++) {
            if (children.at(i).toElement().tagName() == tag) {
                nodes << children.at(i);
            }
        }

        // No node exists of the specified tag - create one...
        if (nodes.isEmpty()) {
            _createChildNode(parent, tag, name);
            parent = parent.lastChild().toElement();
        }

        // Nodes exist of the specified tag - do we need a new one?
        else {

            // Find if the tag already exists with the name
            int iNode = -1;
            for (int n = 0; n < nodes.size(); n++) {
                QString nodeName = nodes.at(n).toElement().attribute(QString("name"));
                if (nodeName == name) {
                    iNode = n;
                }
            }

            // No tag exists with the unique name - append one to the end.
            if (iNode == -1) {
                _createChildNode(parent, tag, name);
                parent = parent.lastChild().toElement();
            }

            // Tag exists - move the parent pointer.
            else {
                parent = nodes.at(iNode).toElement();
            }
        }
    }

    return parent;
}


/**
 * @details
 * Returns a QDomElement at the specified address. The element returned is
 * null if the address doesn't exist.
 */
QDomElement Config::_get(const TreeAddress &address) const
{
    QDomElement parent = _document.documentElement();

    // Empty configuration return null element.
    if (parent.isNull())
        return QDomElement();

    for (int a = 0; a < address.size(); a++) {

        QString tag  = address.at(a).first;
        QString name = address.at(a).second;

        // Find child nodes of the specified tag type.
        QDomNodeList children = parent.childNodes();
        QList<QDomNode> nodes;
        for (int i = 0; i < children.size(); i++) {
            if (children.at(i).toElement().tagName() == tag) {
                nodes << children.at(i);
            }
        }

        // No node exists of the specified tag.
        if (nodes.isEmpty())
            return QDomElement();

        // Nodes exist of the specified tag; find the right one to return.
        else {

            // Find if the tag already exists with the name.
            int iNode = -1;
            for (int n = 0; n < nodes.size(); n++) {
                QString nodeName = nodes.at(n).toElement().attribute(QString("name"));
                if (nodeName == name)
                    iNode = n;
            }

            // Check if no tag exists with the name.
            if (iNode == -1)
                return QDomElement();

            // Tag exists - move the parent pointer.
            else
                parent = nodes.at(iNode).toElement();
        }
    }
    return parent;
}


/**
 * @details
 * Read a configuration from the Pelican XML file specified when the
 * configuration object is instantiated.
 */
void Config::read(const QString fileName)
{
    _fileName = fileName;

    if (fileName.isEmpty()) {
        _document = QDomDocument("pelican");
        return;
    }

    QFile file(fileName);
    if (!file.exists())
        throw QString("Config::read(): "
                "Configuration file \"%1\" not found.").arg(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text))
        return;

    // Read the XML configuration file into the QDomDocument.
    QString error;
    int line, column;
    if (!_document.setContent(&file, true, &error, &line, &column)) {
        throw QString("Config::read(): Parse error "
                "(Line: %1 Col: %2): %3.").arg(line).arg(column).arg(error);
    }

    if (_document.doctype().name() != "pelican")
        throw QString("Config::read(): Invalid doctype (must be \"pelican\".");
}


/**
 * @details
 * Create a child node under the \p parent with the \p tag and \p name attribute.
 */
void Config::_createChildNode(QDomElement &parent, const QString& tag, const QString& name)
{
    if (parent.isNull())
        throw QString("Config: Unable to create child node on null parent.");

    QDomElement e = _document.createElement(tag);
    if (!name.isEmpty())
        e.setAttribute(QString("name"), name);

    parent.appendChild(e);
}

/**
 * @details
 * Pre-processes the current XML document, expanding any <import> tags.
 * The contents of the destination are deep-copied into the current document.
 *
 * Valid use cases are:
 * - @verbatim <import nodeset=" [nodeset name] " /> @endverbatim
 *
 * The following are reserved for future use:
 * - @verbatim <import file=" [file name] " /> @endverbatim
 * - @verbatim <import url=" [address] " /> @endverbatim
 */
void Config::_preprocess()
{
    // Get the list of import nodes.
    QDomNodeList importNodes = _document.elementsByTagName("import");
    std::cout << "Total number of import nodes: " << importNodes.size() << std::endl;
    for (int i = 0; i < importNodes.size(); ++i) {
        // Get the import tag.
        QDomElement importTag = importNodes.at(i).toElement();

        // Check if importing a nodeset.
        QString nodesetName = importTag.attribute("nodeset");
        if (!nodesetName.isEmpty()) {
            // Import the nodeset.
            Config::TreeAddress nodesetAddress;
            nodesetAddress << Config::NodeId("nodesets", "");
            nodesetAddress << Config::NodeId("nodeset", nodesetName);
            QDomElement nodeset = _get(nodesetAddress);

            // Loop over each child in the nodeset and append it.
            QDomNodeList list = nodeset.childNodes();
            for (int j = 0; j < list.size(); j++) {
                importTag.parentNode().appendChild(list.at(j).cloneNode());
            }
        }

        // Check if importing a file.
        QString fileName = importTag.attribute("file");
        if (!fileName.isEmpty()) {
            // Import the file.
            std::cerr << "WARNING: Configuration file import not yet implemented.\n";
        }

    }

    summary();
}

} // namespace pelican
