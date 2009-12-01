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
        /// Constructs the configuration object reading the specified configuration file
        Config(const QString &fileName = "");

        /// Destroys the configuration object.
        ~Config();

        /// Returns the configuration file name
        QString getFileName() const { return _fileName; }

        /// Returns a pointer to the specified configuration node.
        QDomElement& getConfiguration(const QList< QPair<QString, QString> > &address) const;

        /// Creates and returns a configuration option at the specified address.
        QDomElement& setConfiguration(const QList< QPair<QString, QString> > &address);

        /// Sets a configuration option attribute at the specified address.
        void setConfigurationAttribute(
                const QList< QPair<QString, QString> > &address,
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
