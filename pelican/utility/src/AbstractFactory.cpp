#include "AbstractFactory.h"


#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details AbstractFactory
 */
AbstractFactory::AbstractFactory(const Config* config, const Config::TreeAddress& base )
    : _config(config), _configRoot(base)
{
}

/**
 *@details
 */
AbstractFactory::~AbstractFactory()
{
}

ConfigNode AbstractFactory::configuration(const QString& type, const QString& name) const
{
    Config::TreeAddress address = _configRoot;
    address.append(QPair<QString, QString>(type, name));
    ConfigNode element = _config->get(address);
    return element;
}

} // namespace pelican
