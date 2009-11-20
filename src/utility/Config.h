#ifndef PELICAN_CONFIG_H_
#define PELICAN_CONFIG_H_

#include <QDomDocument>
#include <QString>
#include <QStringList>
#include <QHash>

/**
 * @class Config
 *
 * @brief
 * Handles reading and parsing of options for modules and server configuration.
 * 
 * @details
 */

namespace pelican {

class Config
{
    public:
        /// Constructs the configuration object reading the specified configuration file
        Config(const QString &fileName);

        /// Destroys the configuration object.
        ~Config();

        /// Returns an XML configuration node for the specified address tree.
        bool getConfiguration(QStringList address, QDomNode *config);

        /// Set a configuration option for the specifed address tree.
        void setConfigurationOption(QStringList address);

    private:

        /// Checks the configuration file
        bool checkFile();

        /// Reads and parses the configuration file.
        void read();

        /// Check the document type to ensure its a pelican XML file
        void checkDocType();

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
