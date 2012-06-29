#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/Config.h"
#include "pelican/data/DataBlob.h"

#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

#include <cstdlib>
#include <iostream>

namespace pelican {


/**
 * @details
 * Creates a new configuration node.
 */
ConfigNode::ConfigNode(const QDomElement& config, const Config* cfgObject )
    : _configObject(cfgObject)
{
    _config = config;
}


/**
 * @details
 * Creates a new configuration node.
 */
ConfigNode::ConfigNode(const QList<QDomElement>& config)
{
    // Do nothing if list is empty.
    if (config.size() == 0) return;

    // Copy the base node.
    _config = config[0];

    // Loop over additional nodes.
    for (int i = 1; i < config.size(); i++) {

        // Loop over each child in the node set and append it.
        QDomNodeList list = config[i].childNodes();
        for (int j = 0; j < list.size(); j++) {
            _config.appendChild(list.at(j).cloneNode());
        }
    }
}


/**
 * @details
 * Sets the contents of the configuration node from the supplied XML string.
 * Note that the string should contain exactly one top-level XML node.
 *
 * @param xmlString The XML string to use.
 */
void ConfigNode::setFromString(const QString xmlString)
{
   QDomDocument doc;
   QString error;
   int line, column;
   if (!doc.setContent(xmlString, true, &error, &line, &column)) {
       throw QString("ConfigNode::setFromString(): Error parsing XML "
               "(Line: %1 Col: %2): %3.").arg(line).arg(column).arg(error);
   }

   _config = doc.documentElement();
}


/**
 * @details
 * Query function for the top level tag
 * @returns
 * true when the attribute exists, false otherwise
 */
bool ConfigNode::hasAttribute(const QString& attribute) const
{
    return _config.hasAttribute(attribute);
}

/**
 * @details
 * direct access to attributes set in the top level tag
 */
QString ConfigNode::getAttribute(const QString& attribute) const
{
    return _config.attribute(attribute);
}

/**
 * @details
 * Gets the configuration for the given \p tagName and \p attribute.
 * If the \p tagName and/or \p attribute do not exist, \p defValue is returned.
 */
QString ConfigNode::getOption(const QString& tagName,
        const QString& attribute, const QString& defValue) const
{
    return _config.namedItem(tagName).toElement().attribute(attribute,
            defValue);
}


/**
 * @details
 * Gets the configuration for a given \p tagName with name attribute \p name.
 * If the option is not found \p defValue is returned.
 */
QString ConfigNode::getNamedOption(const QString& tagName, const QString& name,
        const QString& attribute, const QString& defValue) const
{
    QDomNode node = _config.namedItem(tagName);
    if (node.isNull()) return defValue;

    QDomNodeList children = node.childNodes();
    for (int c = 0; c < children.size(); c++) {
        QDomElement childElement = children.at(c).toElement();
        if (childElement.tagName() == tagName) {
            if (childElement.attribute("name") == name) {
                return childElement.attribute(attribute);
            }
        }
    }
    return defValue;
}


/**
 * @details
 * Gets the configuration text for the given \p tagName.
 * If the \p tagName and/or text do not exist, \p defValue is returned.
 */
QString ConfigNode::getOptionText(const QString& tagName,
        const QString& defValue) const
{
    QDomNode node = _config.namedItem(tagName);
    if (node.isNull()) return defValue;

    QDomNodeList children = node.childNodes();
    QString text;

    for (int c = 0; c < children.size(); c++) {
        if (children.at(c).nodeType() == QDomNode::TextNode) {
            text += children.at(c).nodeValue();
        }
    }
    return text.isEmpty() ? defValue : text;
}

/**
 * @details
 * Gets a hash of attribute pairs for a list of \p tagname items.
 * The hash is created using the values of the given attributes \p attr1 and
 * \p attr2, where the key is given in \p attr1 and value in \p attr2.
 *
 * Using this XML with tagName="data", attr1="type" and attr2="file",
 *
 * @verbatim
      <data type="VisibilityData" adapter="VisibilityAdapter" file="vis.dat"/>
      <data type="AntennaPositions" adapter="PositionAdapter" file="pos.dat"/>
   @endverbatim
 *
 * would produce a hash containing the following keys and values:
 *
 * @verbatim
      VisibilityData -> vis.dat
      AntennaPositions -> pos.dat
   @endverbatim
 */
QHash<QString, QString> ConfigNode::getOptionHash(const QString& tagName,
        const QString& attr1, const QString& attr2) const
{
    QHash<QString, QString> hash;
    QDomNodeList list = _config.elementsByTagName(tagName);
    for (int i = 0; i < list.size(); ++i) {
        QDomElement element = list.at(i).toElement();
        if (element.hasAttribute(attr1) && element.hasAttribute(attr2)) {
            hash.insert(element.attribute(attr1), element.attribute(attr2));
        }
    }
    return hash;
}

/**
 * @details
 * Gets attributes for a list of \p tagname items.
 * The list is created using the values of the given attribute \p attr.
 *
 * Using this XML with tagName="data" and attr="adapter",
 *
 * @verbatim
      <data type="VisibilityData" adapter="VisibilityAdapter" file="vis.dat"/>
      <data type="AntennaPositions" adapter="PositionAdapter" file="pos.dat"/>
   @endverbatim
 *
 * would produce a list containing the following strings:
 *
 * @verbatim
      VisibilityAdapter
      PositionAdapter
   @endverbatim
 */
QList<QString> ConfigNode::getOptionList(const QString& tagName,
        const QString& attr) const
{
    QList<QString> optionList;
    QDomNodeList list = _config.elementsByTagName(tagName);
    for (int i = 0; i < list.size(); ++i) {
        QDomElement element = list.at(i).toElement();
        if (element.hasAttribute(attr)) {
            optionList.append(element.attribute(attr));
        }
    }
    return optionList;
}

/**
 * @details
 * Returns a vector containing the comma-separated list of unsigned integers
 * inside the given tag name.
 *
 * Channels can be specified in the XML configuration as
 *
 * @verbatim <channels>1,5,182</channels> @endverbatim
 *
 * or, for many contiguous channels,
 *
 * @verbatim <channels start="0" end="511"/> @endverbatim
 */
std::vector<unsigned> ConfigNode::getUnsignedList(const QString& tagName) const
{
    // Declare vector.
    std::vector<unsigned> list;

    // Check if "start" and "end" attributes are present.
    QString startString = getOption(tagName, "start");
    QString endString = getOption(tagName, "end");

    // If start or end are not present, check for text content.
    if (startString.isEmpty() || endString.isEmpty()) {
        QString channelStr = getOptionText(tagName, "0");
        QStringList chanList = channelStr.split(",", QString::SkipEmptyParts);
        unsigned size = chanList.size();
        list.resize(size);
        for (unsigned i = 0; i < size; ++i) {
            list[i] = chanList.at(i).toUInt();
        }
    }

    // Otherwise use start and end.
    else {
        const int beg = startString.toInt();
        const int end = endString.toInt();
        const unsigned size = 1 + abs(end - beg);
        const int dir = (end - beg >= 0) ? 1 : -1;
        list.resize(size);

        // Fill the channel indices.
        for (unsigned i = 0; i < size; ++i) {
            list[i] = beg + (i * dir);
        }
    }

    // Check and throw if list is empty.
    if (list.size() == 0)
        throw QString("%1: No integer list specified.").arg(_config.tagName());
    return list;
}

QList<ConfigNode> ConfigNode::getNodes(const QString& tagName) const
{
    QList<ConfigNode> list;
    QDomNodeList doms = _config.elementsByTagName(tagName);
    for (int i = 0; i < doms.size(); ++i) {
        QDomElement element = doms.at(i).toElement();
        list.append( ConfigNode( element, _configObject) );
    }
    return list;
}
    

QString ConfigNode::searchFile( const QString& filename ) const
{
    return _configObject->searchFile( filename );
}

} // namespace pelican
