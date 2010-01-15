#include <QStringList>
#include <QtGlobal>
#include "ModuleFactory.h"
#include "modules/TestModule.h"
#include "utility/Config.h"
#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * ModuleFactory constructor.
 */
ModuleFactory::ModuleFactory(const Config *config)
{
    _config = config;
    _configRoot.append(Config::NodeId_t("modules", ""));
}

/**
 * @details
 * ModuleFactory destructor.
 */
ModuleFactory::~ModuleFactory()
{
    foreach( AbstractModule* module, _modules ) {
        delete module;
    }
}

/**
 * @details
 * Public interface to create a new module.
 *
 * @param[in] name A string containing the name of the module.
 *
 * @return
 * Returns a pointer to the newly-created module.
 * The object is still owned by the factory.
 */
AbstractModule* ModuleFactory::createModule(const QString& name)
{
    /* Create an index into the configuration file and get the configuration */
    Config::TreeAddress_t address = _configRoot;
    address.append(QPair<QString, QString>("module", name));
    QDomElement element = _config->get(address);

    /* Create the module */
    return _createModule(name, element);
}

/**
 * @details
 * Creates a new module.
 * Called by the public function createModule.
 *
 * @param[in] name   A string containing the name of the module.
 * @param[in] config Configuration of the module.
 *
 * @return
 * Returns a pointer to the newly-created module.
 * The object is still owned by the factory.
 */
AbstractModule* ModuleFactory::_createModule(const QString& name, const QDomElement& config)
{
    AbstractModule* module = 0;
    if( name == "Test" ) {
        module = new TestModule(config);
        _modules.append( module );
    }
    else {
        throw QString("Unknown module type '" + name + "'");
    }
    return module;
}

} // namespace pelican
