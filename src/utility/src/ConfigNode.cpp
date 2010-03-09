#include "utility/ConfigNode.h"
#include <QDomNode>
#include <QDomNodeList>
#include <QTextStream>

namespace pelican {


/**
 * @details
 * Creates a new configuration node.
 */
ConfigNode::ConfigNode(const QDomElement& config)
{
    _config = config;
}


/**
 * @details
 * Creates a new configuration node.
 */
ConfigNode::ConfigNode(const QList<QDomElement>& config)
{
    /* Copy the base node */
    _config = config[0];

    /* Loop over each varset */
    for (int i = 1; i < config.size(); i++) {

        /* Loop over each child in the varset and append it */
        QDomNodeList list = config[i].childNodes();
        for (int j = 0; j < list.size(); j++) {
            _config.appendChild(list.at(j).cloneNode());
        }
    }
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
    QString text = QString();

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
 * \verbatim
      <data type="VisibilityData" adapter="VisibilityAdapter" file="vis.dat"/>
      <data type="AntennaPositions" adapter="PositionAdapter" file="pos.dat"/>
 * \endverbatim
 *
 * would produce a hash containing the following keys and values:
 *
 * \verbatim
      VisibilityData -> vis.dat
      AntennaPositions -> pos.dat
 * \endverbatim
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

} // namespace pelican
