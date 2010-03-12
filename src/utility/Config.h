#ifndef CONFIG_H
#define CONFIG_H

#include "utility/ConfigNode.h"
#include <QDomDocument>
#include <QString>
#include <QStringList>
#include <QHash>

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
        typedef QPair<QString, QString> NodeId_t;
        typedef QList<NodeId_t> TreeAddress_t;

    public:
        /// Constructs the configuration object reading the specified configuration file
        Config(const QString &fileName = "");

        /// Destroys the configuration object.
        ~Config();

        /// Returns the configuration file name
        QString fileName() const { return _fileName; }

        /// Creates and returns a configuration option at the specified address.
        ConfigNode set(const TreeAddress_t &address) {
            return ConfigNode(_set(address));
        }

        /// Returns a pointer to the specified configuration node.
        ConfigNode get(const TreeAddress_t &address) const;

        /// Sets a configuration option attribute at the specified address.
        void setAttribute(const TreeAddress_t &address, const QString &key,
                const QString &value);

        /// Returns the attribute at specified address and key.
        QString getAttribute(const TreeAddress_t& address,
                const QString& key) const;

        /// Set the text node at the specified address.
        void setText(const TreeAddress_t& address, const QString& text);

        /// Returns the text node at the specified address.
        QString getText(const TreeAddress_t& address) const;

        /// Prints a summary of the configuration to specified tree depth
        void summary() const;

        /// Saves the configuration to the specified file name
        void save(const QString& fileName) const;

        /// Sets the configuration from the QString text.
        /// Warning: This method is added for testing only and will destroy
        /// any previous configuration.
        void setFromString(const QString& text);

    protected:
        /// Reads and parses the configuration file.
        void read(QString fileName);

    private:
        /// Returns a pointer to the specified configuration node.
        QDomElement _get(const TreeAddress_t &address) const;

        /// Creates and returns a configuration option at the specified address.
        QDomElement _set(const TreeAddress_t &address);

        /// Creates a child configuration node
        void _createChildNode(QDomElement &parent, const QString& tag,
                const QString& name);

    private:
        QString _fileName;
        QDomDocument _document;
};

} // namespace pelican

#endif /* CONFIG_H */
