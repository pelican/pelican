#include "Config.h"
#include <QTextStream>
#include <QFile>
//#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Configuration constructor.
 */
Config::Config(QString fileName)
{
    _fileName = fileName;
} /* Config() */


/**
 * @details
 * Configuration destructor.
 */
Config::~Config()
{
} /* ~Config() */


/**
 * @details
 */
bool Config::read()
{
    QTextStream out(stdout), err(stderr);

    /* Open the configuration file for reading */
    if (_fileName.isEmpty()) return false;
    out << "= Reading = \"" << _fileName << "\"" << endl;
    
    _domDocument.clear();
    QFile file(_fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) return false;
    
    /* Parse the XML document from the byte array data and sets it
       as the contents of the document */
    QString errStr; int errLine, errColumn;
    if (!_domDocument.setContent(&file, true, &errStr, &errLine, &errColumn)) {
            err << "!! Parse error: line " << errLine << " column " << errColumn
                << " :" << errStr << endl;
        return false;
    }

    /* Check the doctype to ensure this is a pelican configuration XML file */ 
    if (_domDocument.doctype().name() != "pelican") {
        err << "!! Configuration file is not a pelican configuration document!" << endl;
        _domDocument.clear();
        return false;
    }
    
    /* Store a QHash lookup table for server configuration */
    out << "= Found servers: " << endl;
    _servers = _domDocument.elementsByTagName("server");
    for (int i = 0; i < _servers.size(); i++) {
        QDomElement e = _servers.at(i).toElement();
        QString type = e.attribute("type");
        QString name = e.attribute("name");
        QString id = type + "::" + name;
        _serverList[id] = i;
        out << " - type = \"" << type << "\"  " << "name = \"" << name << "\"  "
            << "id = \"" << id << "\"" << endl;
    }

    /* Store a QHash lookup table for module configuration */
    out << "= Found modules: " << endl;
    _modules = _domDocument.elementsByTagName("module");
    for (int i = 0; i < _modules.size(); i++) {
        QDomElement e = _modules.at(i).toElement();
        QString type = e.attribute("type");
        QString name = e.attribute("name");
        QString id = type + "::" + name;
        _moduleList[id] = i;
        out << " - type = \"" << type << "\"  " << "name = \"" << name << "\"  "
            << "id = \"" << id << "\"" << endl;
    }
    out << "= Read Complete." << endl;
    return true;
} /* read() */



/**
 * @details
 */
bool Config::getModuleConfiguration(QString moduleId, QDomNode *config)
{
    if (!_moduleList.contains(moduleId)) return false;
    *config = _modules.at(_moduleList[moduleId]);
    return true;
}


} /* namespace pelican */
