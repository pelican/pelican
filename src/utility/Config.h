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
        QDomNode* getConfiguration(QStringList address);

        /// Sets a configuration option
        void setConfigurationOption();

    private:

        /// Reads and parses the configuration file.
        void read();

        /// Parse the server configuration and populate a QHash lookup
        void parseServerConfig();

        /// Parse the server configuration and populate a QHash lookup
        void parseModuleConfig();

    private:
        QString _fileName;
        QDomDocument _domDocument;
        QDomNodeList _modules;
        QHash<QString, int> _moduleList;
        QDomNodeList _servers;
        QHash<QString, int> _serverList;
};




} /* namespace pelican */

#endif /* PELICAN_CONFIG_H_ */
