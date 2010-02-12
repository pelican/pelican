#ifndef CONFIGNODE_H_
#define CONFIGNODE_H_

#include <QDomElement>
#include <QString>

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
    public:
        /// Constructs the configuration object reading the specified configuration file
        ConfigNode(const QDomElement& config);

        /// Destroys the configuration object.
        ~ConfigNode();

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

    private:
        QDomElement _config;
};

} /* namespace pelican */

#endif /* CONFIGNODE_H_ */
