#ifndef PELICAN_CONFIG_H_
#define PELICAN_CONFIG_H_

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
    public:
        typedef QPair<QString, QString> NodeId_t;
        typedef QList<NodeId_t> TreeAddress_t;

    public:

        /// Constructs the configuration object reading the specified configuration file
        Config(const QString &fileName = "");

        /// Destroys the configuration object.
        ~Config();

        /// Returns the configuration file name
        QString getFileName() const { return _fileName; }

        /// Creates and returns a configuration option at the specified address.
        QDomElement set(const TreeAddress_t &address);

        /// Returns a pointer to the specified configuration node.
        const QDomElement get(const TreeAddress_t &address) const;

        /// Sets a configuration option attribute at the specified address.
        void setAttribute(
                const TreeAddress_t &address,
                const QString &key,
                const QString &value
        );

        /// Prints a summary of the configuration to specified tree depth
        void summary() const;

    private:
        /// Reads and parses the configuration file.
        void _read();

        /// Creates a child configuration node
        void _createChildNode(QDomElement &parent, const QString& tag, const QString& name);

        /// Prints children of a element node recursively up to the max depth.
        void _printChildren(const QDomElement& element, const int depth, const int maxDepth = 4) const;


    private:
        QString _fileName;
        QDomDocument _document;
};


} /* namespace pelican */

#endif /* PELICAN_CONFIG_H_ */
