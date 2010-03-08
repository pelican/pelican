#include "AbstractFactory.h"


#include "utility/memCheck.h"

namespace pelican {


/**
 *@details AbstractFactory 
 */
AbstractFactory::AbstractFactory(const Config* config, const Config::TreeAddress_t& base )
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
    Config::TreeAddress_t address = _configRoot;
    address.append(QPair<QString, QString>(type, name));
    ConfigNode element = _config->get(address);
    return element;
}

} // namespace pelican
