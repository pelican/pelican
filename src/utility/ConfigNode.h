#ifndef CONFIGNODE_H_
#define CONFIGNODE_H_

#include <QDomElement>
#include <QString>
#include <QHash>
#include <QList>

namespace pelican {

/**
 * @class ConfigNode
 *
 * @brief
 * Wraps and handles extracting settings from a QDomElement
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

        /// Destroys the configuration object.
        ~ConfigNode() {}

    public:
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

        /// Returns the QDomElement that this class wraps.
        const QDomElement getDomElement() const {return _config;}
};

} /* namespace pelican */

#endif /* CONFIGNODE_H_ */
