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
        typedef QList< QPair<QString, QString> > TreeAddress_t;

    public:

        /// Constructs the configuration object reading the specified configuration file
        Config(const QString &fileName = "");

        /// Destroys the configuration object.
        ~Config();

        /// Returns the configuration file name
        QString getFileName() const { return _fileName; }

        /// Creates and returns a configuration option at the specified address.
        QDomElement& setConfiguration(const TreeAddress_t &address);

        /// Returns a pointer to the specified configuration node.
        QDomElement& getConfiguration(const TreeAddress_t &address) const;

        /// Sets a configuration option attribute at the specified address.
        void setConfigurationAttribute(
                const TreeAddress_t &address,
                const QString &key,
                const QString &value
        );

    private:
        /// Reads and parses the configuration file.
        void read();

        /// Parse the server configuration and populate a QHash lookup
        void parseServerConfig();

        /// Parse the server configuration and populate a QHash lookup
        void parseModuleConfig();

    private:
        QString _fileName;
        QDomDocument _document;
        QDomNodeList _modules;
        QDomNodeList _servers;
        QHash<QString, int> _moduleList;
        QHash<QString, int> _serverList;
};




} /* namespace pelican */

#endif /* PELICAN_CONFIG_H_ */
