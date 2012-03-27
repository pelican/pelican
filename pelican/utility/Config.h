#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file Config.h
 */

#include "pelican/utility/ConfigNode.h"
#include <QtXml/QDomDocument>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QFile>

namespace pelican {

/**
 * @ingroup c_utility
 *
 * @class Config
 *
 * @brief
 * Handles reading and parsing of options for modules and server configuration.
 *
 * @details
 */

class Config
{
    private:
        friend class ConfigTest;

    public:
        /// Convenience types for holding configuration addresses.
        typedef QPair<QString, QString> NodeId;
        typedef QList<NodeId> TreeAddress;

    public:
        /// Constructs the configuration object reading the specified configuration file
        Config(const QString &fileName = "");

        /// Destroys the configuration object.
        virtual ~Config() {};

    public:
        /// Returns the configuration file name
        QString fileName() const { return _fileName; }

        /// Returns the configuration node at the specified address.
        ConfigNode get(const TreeAddress &address) const {
            return ConfigNode(_get(address, _document), this );
        }

        /// returns true if a node at the specified address exists
        bool verifyAddress( const TreeAddress &address) const;

        /// Returns the attribute at specified address and key.
        QString getAttribute(const TreeAddress& address,
                const QString& key) const;

        /// Returns the text node at the specified address.
        QString getText(const TreeAddress& address) const;

        /// Imports a file.
        void importFile(QDomDocument& document,
                QDomNode& parent, const QString& name);

        /// Search the configuratipn path for the specified
        //  file.
        QString searchFile( const QString& filename ) const;

        /// set the search paths for imported configuration files
        void setSearchPaths( const QList<QString>& );

        /// Imports a nodeset.
        void importNodeset(QDomDocument& document,
                QDomNode& parent, const QString& name);

        /// Imports a URL.
        void importUrl(QDomDocument& document,
                QDomNode& parent, const QString& url);

        /// Pre-processes the XML document.
        void preprocess(QDomDocument& document);

        /// Saves the configuration to the specified file name
        void save(const QString& fileName) const;

        /// Creates and returns a configuration option at the specified address.
        ConfigNode set(const TreeAddress &address) {
            return ConfigNode(_set(address), this );
        }

        /// Sets a configuration option attribute at the specified address.
        void setAttribute(const TreeAddress &address, const QString &key,
                const QString &value);

        /// Sets the document.
        void setDocument(const QDomDocument& document) {_document = document;}

        /// Sets the configuration from the QString text.
        /// Warning: This method is added for testing only and will destroy
        /// any previous configuration.
        void setFromString(const QString& pipelineConfig,
                const QString& serverConfig = "",
                const QString& nodesets = "");

        /// Sets the document as actually specifed without the machinations
        //  of the setFromString method
        void setXML( const QString& xml );

        /// Set the text node at the specified address.
        void setText(const TreeAddress& address, const QString& text);

        /// Prints a summary of the configuration.
        void summary() const;

    protected:
        /// Reads and parses the configuration file.
        QDomDocument read(const QString& fileName);

    private:
        /// Creates a child configuration node
        void _createChildNode(QDomElement &parent, const QString& tag,
                const QString& name);

        /// Returns a pointer to the specified configuration node.
        QDomElement _get(const TreeAddress &address,
                const QDomDocument& document) const;

        /// Creates and returns a configuration option at the specified address.
        QDomElement _set(const TreeAddress &address);

    private:
        QList<QString> _searchPaths; // prefix for the Qt resource search
        QString _fileName;
        QDomDocument _document;
};

} // namespace pelican

#endif // CONFIG_H
