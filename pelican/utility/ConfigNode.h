#ifndef CONFIGNODE_H_
#define CONFIGNODE_H_

/**
 * @file ConfigNode.h
 */

#include "pelican/utility/constants.h"
#include <QDomElement>
#include <QString>
#include <QHash>
#include <QStringList>
#include <QList>
#include <vector>

namespace pelican {

/**
 * @class ConfigNode
 *
 * @brief
 * Wraps and handles extracting settings from a settings node in a pelican XML
 * configuration file.
 *
 * @details
 */

class ConfigNode
{
    private:
        QDomElement _config;

    public:
        /// Constructs an empty configuration node.
        ConfigNode() {}

        /// Constructs the configuration node from the specified QDomElement.
        ConfigNode(const QDomElement& config);

        /// Constructs the configuration node from the specified QDomElement list.
        ConfigNode(const QList<QDomElement>& config);

        /// Constructs the configuration node from the XML string.
        ConfigNode(const QString& xmlString) { setFromString(xmlString); }

        /// Destroys the configuration object.
        ~ConfigNode() {}

    public:
        /// Returns the configuration node type.
        QString type() const { return _config.tagName(); }

        /// Returns the configuration node name.
        QString name() const { return _config.attribute("name"); }

        /// Returns the name of the parent node.
        QString parentName() const {
            return _config.parentNode().toElement().tagName();
        }

        /// Sets the node from the xml string (NOTE: use for testing)
        void setFromString(const QString xmlString);

        /// Returns a configuration option (attribute).
        /// i.e. the attribute for the XML: <tagName attribute=""/>
        QString getOption(const QString& tagName, const QString& attribute,
                const QString& defValue = QString()) const;

        /// Returns a configuration option (attribute).
        /// i.e. the attribute for the XML: <tagName name="" attribute=""/>
        QString getNamedOption(const QString& tagName, const QString& name,
                const QString& attribute,
                const QString& defValue = QString()) const;

        /// Returns a configuration option text string value.
        /// i.e. the text node for <tagName>text</tagName>
        QString getOptionText(const QString& tagName,
                const QString& defValue = QString()) const;

        /// Returns a hash of attribute pairs.
        QHash<QString, QString> getOptionHash(const QString& tagName,
                const QString& attr1, const QString& attr2) const;

        /// Returns a list of attributes.
        QStringList getOptionList(const QString& tagName,
                const QString& attr) const;

        /// Returns the QDomElement that this class wraps.
        const QDomElement getDomElement() const {return _config;}

        /// Returns the list of unsigned integers, if any.
        std::vector<unsigned> getUnsignedList(const QString& tagName) const;

        /// Returns the polarisation, if any.
        Polarisation getPolarisation(
                const QString& tagName = QString("polarisation")) const;
};

} // namespace pelican

#endif // CONFIGNODE_H_
