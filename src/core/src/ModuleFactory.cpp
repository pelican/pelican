#include <QStringList>
#include "ModuleFactory.h"
#include "modules/TestModule.h"
#include "utility/Config.h"
#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * ModuleFactory constructor.
 */
ModuleFactory::ModuleFactory(Config *config)
{
    _config = config;
    _moduleAddress.append("modules");
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
 * @return
 * Returns a pointer to the newly-created module.
 * The object is still owned by the factory.
 */
AbstractModule* ModuleFactory::createModule(const QString& name)
{
    QStringList moduletree = _moduleAddress;
    moduletree.append(name);
    QDomNode* node = _config->getConfiguration(moduletree);
    return _createModule(name, *node );
}

/**
 * @details
 * Creates a new module.
 */
AbstractModule* ModuleFactory::_createModule(const QString& type, const QDomNode& config)
{
    AbstractModule* module = 0;
    if( type == "Test" ) {
        module = new TestModule(config);
        _modules.append( module );
    }
    else {
        throw QString("Unknown module type '" + type + "'");
    }
    return module;
}

} // namespace pelican
