#include "utility/ConfigNode.h"
#include "data/DataBlob.h"
#include <QDomNode>
#include <QDomNodeList>
#include <QStringList>
#include <QTextStream>
#include <cstdlib>

#include "utility/memCheck.h"

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
    // Copy the base node.
    _config = config[0];

    // Loop over each varset.
    for (int i = 1; i < config.size(); i++) {

        // Loop over each child in the varset and append it.
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

/**
 * @details
 * Returns a vector containing the list of channels, if any.
 * Channels are specified in the XML configuration as
 *
 * \verbatim
      <channels>1,5,182</channels>
 * \endverbatim
 *
 * or, for many contiguous channels,
 *
 * \verbatim
      <channels start="0" end="511"/>
 * \endverbatim
 */
std::vector<unsigned> ConfigNode::getChannels() const
{
    // Declare channel vector.
    std::vector<unsigned> channels;

    // Check if "start" and "end" tags are present.
    const QString channelTag("channels");
    const int beg = getOption(channelTag, "start", "-1").toInt();
    const int end = getOption(channelTag, "end", "-1").toInt();

    // If start or end are negative, check for text content.
    if (beg < 0 || end < 0) {
        QString channelStr = getOptionText(channelTag, "0");
        QStringList chanList = channelStr.split(",", QString::SkipEmptyParts);
        unsigned size = chanList.size();
        channels.resize(size);
        for (unsigned i = 0; i < size; ++i) {
            channels[i] = chanList.at(i).toUInt();
        }
    }

    // Otherwise use start and end channels.
    else {
        const unsigned size = 1 + abs(end - beg);
        const int dir = (end - beg >= 0) ? 1 : -1;
        channels.resize(size);

        // Fill the channel indices.
        for (unsigned i = 0; i < size; ++i) {
            channels[i] = beg + (i * dir);
        }
    }

    // Check and throw if there are no channels.
    if (channels.size() == 0)
        throw QString("%1: No channels specified.").arg(_config.tagName());
    return channels;
}

/**
 * @details
 * Returns the polarisation option.
 */
DataBlob::pol_t ConfigNode::getPolarisation() const
{
    QString pol = getOption("polarisation", "value", "x").toLower();
    if (pol.startsWith("x"))
        return DataBlob::POL_X;
    else if (pol.startsWith("y"))
        return DataBlob::POL_Y;
    else if (pol.startsWith("both"))
        return DataBlob::POL_BOTH;
    else
        throw QString("%1: Unknown polarisation.").arg(_config.tagName());

    return DataBlob::POL_UNDEF;
}

} // namespace pelican
