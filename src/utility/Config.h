#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file Config.h
 */

#include "utility/ConfigNode.h"
#include <QDomDocument>
#include <QString>
#include <QHash>
#include <QPair>

namespace pelican {

/**
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

        /// Creates and returns a configuration option at the specified address.
        ConfigNode set(const TreeAddress &address) {
            return ConfigNode(_set(address));
        }

        /// Returns a pointer to the specified configuration node.
        ConfigNode get(const TreeAddress &address) const;

        /// Sets a configuration option attribute at the specified address.
        void setAttribute(const TreeAddress &address, const QString &key,
                const QString &value);

        /// Returns the attribute at specified address and key.
        QString getAttribute(const TreeAddress& address,
                const QString& key) const;

        /// Set the text node at the specified address.
        void setText(const TreeAddress& address, const QString& text);

        /// Returns the text node at the specified address.
        QString getText(const TreeAddress& address) const;

        /// Prints a summary of the configuration to specified tree depth
        void summary() const;

        /// Saves the configuration to the specified file name
        void save(const QString& fileName) const;

        /// Sets the configuration from the QString text.
        /// Warning: This method is added for testing only and will destroy
        /// any previous configuration.
        void setFromString(const QString& pipelineConfig,
                           const QString& serverConfig = "");

    protected:
        /// Reads and parses the configuration file.
        void read(QString fileName);

    private:
        /// Returns a pointer to the specified configuration node.
        QDomElement _get(const TreeAddress &address) const;

        /// Creates and returns a configuration option at the specified address.
        QDomElement _set(const TreeAddress &address);

        /// Creates a child configuration node
        void _createChildNode(QDomElement &parent, const QString& tag,
                const QString& name);

    private:
        QString _fileName;
        QDomDocument _document;
};

} // namespace pelican

#endif /* CONFIG_H */
