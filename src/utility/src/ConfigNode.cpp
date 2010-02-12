#include "utility/ConfigNode.h"
#include <QDomNode>
#include <QDomNodeList>

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
 * Destroys the configuration node.
 */
ConfigNode::~ConfigNode()
{
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

} // namespace pelican

