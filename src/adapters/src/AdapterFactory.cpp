#include "AdapterFactory.h"
#include "adapters/AbstractAdapter.h"
#include "adapters/AbstractStreamAdapter.h"
#include "adapters/AbstractServiceAdapter.h"
#include "utility/ConfigNode.h"
#include "utility/Config.h"
#include <QStringList>
#include <QtGlobal>

#include "adapters/AdapterAntennaList.h"
#include "adapters/AdapterLofarStationVisibilities.h"

#include "utility/memCheck.h"

namespace pelican {


/**
 * @details
 * AdapterFactory constructor.
 */
AdapterFactory::AdapterFactory(const Config *config)
{
    _config = config;
    _configRoot.append(Config::NodeId_t("pipeline", ""));
    _configRoot.append(Config::NodeId_t("adapters", ""));
}


/**
 * @details
 * AdapterFactory destructor.
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
    // Create an index into the configuration file and get the configuration.
    Config::TreeAddress_t address = _configRoot;
    address.append(QPair<QString, QString>(type, name));
    ConfigNode config = _config->get(address);

    // Create the adapter.
    return _create(type, config);
}


/**
 * @details
 * Creates a new adapter.
 * Called by the public function create().
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
    if (type == "AdapterLofarStationVisibilities") {
        adapter = new AdapterLofarStationVisibilities(config);
        _adapters.append(adapter);
    }
    else if (type == "AdapterAntennaList") {
        adapter = new AdapterAntennaList(config);
        _adapters.append(adapter);
    }
    else {
        throw QString("AdapterFactory: Unknown adapter type '" + type + "'");
    }
    return adapter;
}

} // namespace pelican
