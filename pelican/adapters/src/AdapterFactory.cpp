#include "pelican/adapters/AdapterFactory.h"
#include "pelican/adapters/AbstractAdapter.h"
#include "pelican/adapters/AbstractStreamAdapter.h"
#include "pelican/adapters/AbstractServiceAdapter.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/Config.h"

#include <QStringList>
#include <QtGlobal>

#include "pelican/adapters/AdapterAntennaList.h"
#include "pelican/adapters/AdapterLofarStationVisibilities.h"

#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 * @details
 * AdapterFactory constructor.
 */
AdapterFactory::AdapterFactory(const Config *config)
{
    _config = config;
    _configRoot.append(Config::NodeId("pipeline", ""));
    _configRoot.append(Config::NodeId("adapters", ""));
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
    Config::TreeAddress address = _configRoot;
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
