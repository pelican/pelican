#include "pelican/utility/Config.h"

#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtXml/QDomDocumentType>

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

namespace pelican {

/*=============================================================================
 * CONSTRUCTOR
 *---------------------------------------------------------------------------*/

/**
 * @details
 * Configuration constructor.
 */
Config::Config(const QString& fileName)
    : _fileName(fileName)
{
    // set the search path for configuration files
    _searchPaths.append( QDir::currentPath() );

    // read in the document
    _document = read(fileName);
}

/*=============================================================================
 * PUBLIC MEMBERS
 *---------------------------------------------------------------------------*/

/**
 * @details
 * Returns the attribute at the address with the specified key. An empty string
 * is returned if the address or key doesn't exist.
 */
QString Config::getAttribute(const TreeAddress& address, const QString& key) const
{
    QDomElement e = _get(address, _document);
    if (e.isNull())
        return QString();
    else
        return e.attribute(key);
}

/* we use QList<QString> in place of QStringList in the main interface incase we ever need
 * to go to support versions < 4.3
 */
void Config::setSearchPaths( const QList<QString>& paths ) {
     _searchPaths.clear();
     foreach( const QString& path, paths ) {
        if( ! path.endsWith( QDir::separator() ) ) {
            _searchPaths.append( path + QDir::separator() );
        }
        else {
            _searchPaths.append( path );
        }
     }
}

bool Config::verifyAddress( const TreeAddress &address) const
{
    QDomElement e = _get(address, _document);
    return ! e.isNull();
}

/**
 * @details
 * Gets value of the text node at the address. If multiple text nodes are
 * found at the same level they will be concatenated.
 */
QString Config::getText(const TreeAddress& address) const
{
    QDomElement e = _get(address, _document);
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
 * Imports and appends a file to the given \p parent XML node.
 * All element nodes under (but not including) the root node of the
 * file are copied to the \p document.
 *
 * If not an absolute path, the name of the file to import must be relative
 * to the executable.
 *
 * @param[in] document A reference to the XML document.
 * @param[in] parent   A reference to the parent XML node.
 * @param[in] name     Name of the file to import.
 */
void Config::importFile(QDomDocument& document,
        QDomNode& parent, const QString& name)
{
    // Return if name is empty.
    if (name.isEmpty()) return;

    // find the file
    QString filename = searchFile(name);

    // Write comment.
    parent.appendChild(document.createComment(
            "% Imported file " + filename) );

    // Import the file.
    QDomDocument newDoc = read(filename);

    // Loop over each child under the root node of the file, and append it.
    QDomNodeList list = newDoc.documentElement().childNodes();
    for (int j = 0; j < list.size(); j++) {
        parent.appendChild(list.at(j).cloneNode());
    }
}

QString Config::searchFile( const QString& file ) const {
    // check the filename
    // must put in absoulte check otherwise will return files in cwd
    // which may not be in the path
    if( QDir::isAbsolutePath(file) && QFile::exists( file ) ) return file;

    // search the configuration path
    foreach( const QString& path, _searchPaths ) {
       QString tmp = path + file;
       if( QFile::exists( tmp ) ) return tmp;
    }
    throw QString("unable to find config file \"%1\""
            " in (").arg(file) + QStringList(_searchPaths).join(",") + ")";
}


/**
 * @details
 * Imports and appends a nodeset from the current \p document to the
 * given \p parent XML node.
 *
 * @param[in] document A reference to the XML document.
 * @param[in] parent   A reference to the parent XML node.
 * @param[in] name     Name of the nodeset to import.
 */
void Config::importNodeset(QDomDocument& document,
        QDomNode& parent, const QString& name)
{
    // Return if name is empty.
    if (name.isEmpty()) return;

    // Write comment.
    parent.appendChild(document.createComment(
            "% Imported nodeset " + name) );

    // Import the nodeset.
    Config::TreeAddress nodesetAddress;
    nodesetAddress << Config::NodeId("nodesets", "");
    nodesetAddress << Config::NodeId("nodeset", name);
    QDomElement nodeset = _get(nodesetAddress, document);

    // Loop over each child in the nodeset and append it.
    QDomNodeList list = nodeset.childNodes();
    for (int j = 0; j < list.size(); j++) {
        parent.appendChild(list.at(j).cloneNode());
    }
}

/**
 * @details
 * Imports and appends a URL to the given \p parent XML node.
 * Note that this functionality is not yet implemented.
 *
 * @param[in] document A reference to the XML document.
 * @param[in] parent   A reference to the parent XML node.
 * @param[in] url      URL to import.
 */
void Config::importUrl(QDomDocument& document,
        QDomNode& parent, const QString& url)
{
    // Return if URL is empty.
    if (url.isEmpty()) return;

    // Write comment.
    parent.appendChild(document.createComment(
            "% Could not import URL " + url) );

    // Import the URL.
    std::cerr << "Configuration URL import not yet implemented.\n";
}

/**
 * @details
 * Pre-processes the given XML document, expanding any <import> tags.
 * The contents of the destination are deep-copied into the current document.
 *
 * This pre-processor will add comments starting with '%' to record where
 * nodes have been imported. On import, nodes are appended to the end of the
 * current list of child nodes, so that local settings can override them.
 *
 * Valid use cases are:
 * - @verbatim <import nodeset=" [nodeset name] " /> @endverbatim
 * - @verbatim <import file=" [file name] " /> @endverbatim
 *
 * The following are reserved for future use:
 * - @verbatim <import url=" [address] " /> @endverbatim
 *
 * @param[in] document A reference to the document to process.
 */
void Config::preprocess(QDomDocument& document)
{
    for (;;) {
        // Get the list of import nodes.
        QDomNodeList importNodes = document.elementsByTagName("import");
        if (importNodes.size() == 0) break;
        for (int i = 0; i < importNodes.size(); ++i) {
            // Get the import tag.
            QDomElement tag = importNodes.at(i).toElement();
            QDomNode parent = tag.parentNode();

            // Perform import as required.
            importNodeset(document, parent, tag.attribute("nodeset"));
            importFile(document, parent, tag.attribute("file"));
            importUrl(document, parent, tag.attribute("url"));
        }

        // Remove the import nodes.
        int nNodes = importNodes.size();
        for (int i = 0; i < nNodes; ++i) {
            QDomNode parent = importNodes.at(0).parentNode();
            parent.removeChild(importNodes.at(0));
        }
    }
}

/**
 * @details
 * Save the Configuration to specified file name
 */
void Config::save(const QString& fileName) const
{
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return;

    QTextStream out(&file);
    out << _document.toByteArray(4);
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
 * sets the contents from the xml provided in the string
 */
void Config::setXML( const QString& xml )
{
    _document.clear();
    _document = QDomDocument("pelican");

    // Read the XML configuration file into the QDomDocument.
    QString error;
    int line, column;

    if (!_document.setContent( xml, &error, &line, &column)) {
        throw QString("Config::setXML(): Error parsing XML "
                "(Line: %1 Col: %2): %3.\n"
                "-----------------------\n"
                "%4\n---------------------\n").
                arg(line).arg(column).arg(error).arg(xml);
    }
    preprocess(_document);
}

/**
 * @details
 * Sets the content of the configuration QDomDocument from the QStrings
 * \p pipelineConfig, \p serverConfig and \p nodesets.
 *
 * @param[in] pipelineConfig String containing the pipeline XML configuration.
 * @param[in] serverConfig String containing server XML configuration.
 */
void Config::setFromString(const QString& pipelineConfig,
        const QString& serverConfig, const QString& nodesets)
{
    _document.clear();
    _document = QDomDocument("pelican");

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
            "<nodesets>"
            "" + nodesets + ""
            "</nodesets>"
            "</configuration>";

    setXML( xmlDoc );
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


/*=============================================================================
 * PROTECTED MEMBERS
 *---------------------------------------------------------------------------*/

/**
 * @details
 * Read a configuration from the Pelican XML file specified when the
 * configuration object is instantiated.
 */
QDomDocument Config::read(const QString& fileName)
{
    QDomDocument document("pelican");
    if (fileName.isEmpty()) return document;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        throw QString("Config::read(): Cannot open file '%1'").arg(fileName);

    // Read the XML configuration file into the QDomDocument.
    QString error;
    int line, column;
    if (!document.setContent(&file, true, &error, &line, &column)) {
        throw QString("Config::read(): Parse error "
                "(Line: %1 Col: %2): %3.").arg(line).arg(column).arg(error);
    }

    if (document.doctype().name() != "pelican")
        throw QString("Config::read(): Invalid doctype (must be 'pelican').");

    // Pre-process and return the document.
    preprocess(document);
    return document;
}

/*=============================================================================
 * PRIVATE MEMBERS
 *---------------------------------------------------------------------------*/

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
 * Returns a QDomElement at the specified address in the given document.
 * The element returned is null if the address doesn't exist.
 *
 * @param[in] address The address of the configuration node to get.
 *
 * @return A QDomElement at the address specified in the argument.
 */
QDomElement Config::_get(const TreeAddress &address, const QDomDocument& document) const
{
    QDomElement parent = document.documentElement();

    // Empty configuration, so return null element.
    if (parent.isNull())
        return QDomElement();

    int index = 0;
    // addresses can refer from the top node, or one level below the
    // top node. Need to check which one.
    if( address.at(0).first == parent.tagName() ) ++index;


    for (int a = index; a < address.size(); a++)
    {
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
        {
//            cerr << "Config::_get(): No node exists." << endl;
//            cerr << "tag = '" << tag.toStdString() << "'" << endl;
//            cerr << "name = '" << name.toStdString() << "'" << endl;
//            cerr << "parent = '" << parent.tagName().toStdString() << "'"<< endl;
//            cerr << endl;
            return QDomElement();
        }

        // Nodes exist of the specified tag; find the right one to return.
        else
        {

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
 * Sets and creates the specified address in the configuration document.
 *
 * @param[in] address The address of the configuration node element to be set.
 *
 * @return A QDomElement at the address specified in the argument.
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

} // namespace pelican
