#ifndef PELICAN_CONFIG_H_
#define PELICAN_CONFIG_H_

#include <QDomDocument>
#include <QString>
#include <QHash>

/**
 * @class Config
 *
 * @brief
 * Test class to read a pelican config XML file using Qt's DOM based XML API.
 * 
 * @details
 * Configuration file handling.
 */

namespace pelican {

class Config
{
    public:
        /// Constructs the configuration object.
        Config(QString fileName);

        /// Destroys the configuration object.
        ~Config();

        /// Reads and parses the configuration file.
        bool read();

        /// Returns the XML configuration node for the specified module
        bool getModuleConfiguration(QString moduleId, QDomNode *config);

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
