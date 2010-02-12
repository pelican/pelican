#include "AdapterFactory.h"
#include "adapters/AbstractAdapter.h"
#include "adapters/FileAdapterLofarVisibilities.h"
#include "utility/ConfigNode.h"
#include "utility/Config.h"
#include <QStringList>
#include <QtGlobal>

#include "utility/memCheck.h"

namespace pelican {


/**
 * @details
 * ModuleFactory constructor.
 */
AdapterFactory::AdapterFactory(const Config *config)
{
    _config = config;
    _configRoot.append(Config::NodeId_t("adapters", ""));
}


/**
 * @details
 * ModuleFactory destructor.
 */
AdapterFactory::~AdapterFactory()
{
    foreach (AbstractAdapter* adapter, _adapters) {
        delete adapter;
    }
}


/**
 * @details
 * Public interface to create a new adapter.
 *
 * @param[in] type  A string containing the type of the adapter.
 * @param[in] name  A string containing the name of the adapter.
 *
 * @return
 * Returns a pointer to the newly-created adapter.
 * The object is still owned by the factory.
 */
AbstractAdapter* AdapterFactory::create(const QString& type, const QString& name)
{
    /* Create an index into the configuration file and get the configuration */
    Config::TreeAddress_t address = _configRoot;
    address.append(QPair<QString, QString>(type, name));
    const ConfigNode config = _config->get(address);

    /* Create the module */
    return _create(name, config);
}


/**
 * @details
 * Creates a new adapter.
 * Called by the public function createAdapter.
 *
 * @param[in] type   A string containing the type of the adapter.
 * @param[in] config Configuration node for the adapter.
 *
 * @return
 * Returns a pointer to the newly-created adapter.
 * The object is still owned by the factory.
 */
AbstractAdapter* AdapterFactory::_create(const QString& type, const ConfigNode& config)
{
    AbstractAdapter* adapter = 0;
    if (type == "FileAdatperLofarVisibilties") {
        adapter = new FileAdapterLofarVisibilities(config);
        _adapters.append(adapter);
    }
    else throw QString("Unknown adapter type '" + type + "'");
    return adapter;
}


} // namespace pelican
