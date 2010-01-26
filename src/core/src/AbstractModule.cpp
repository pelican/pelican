#include "core/AbstractModule.h"

namespace pelican {

/**
 * @details
 * Creates a new abstract Pelican module with the given configuration.
 */
AbstractModule::AbstractModule(const QDomElement& config)
{
    _config = config;
}

/**
 * @details
 * Destroys the module.
 */
AbstractModule::~AbstractModule()
{
}

/**
 * @details
 * The given \p list is added to the local service data required by the
 * module.
 */
void AbstractModule::addLocalServiceData(const QSet<QString>& list)
{
    _reqLocal.addServiceData(list);
}

/**
 * @details
 * The given parameter is added to the local service data required by the
 * module.
 */
void AbstractModule::addLocalServiceData(const QString& string)
{
    _reqLocal.setServiceData(string);
}

/**
 * @details
 * The given \p list is added to the local stream data required by the
 * module.
 */
void AbstractModule::addLocalStreamData(const QSet<QString>& list)
{
    _reqLocal.addStreamData(list);
}

/**
 * @details
 * The given parameter is added to the local stream data required by the
 * module.
 */
void AbstractModule::addLocalStreamData(const QString& string)
{
    _reqLocal.setStreamData(string);
}

/**
 * @details
 * The given \p list is added to the remote service data required by the
 * module.
 */
void AbstractModule::addRemoteServiceData(const QSet<QString>& list)
{
    _reqRemote.addServiceData(list);
}

/**
 * @details
 * The given parameter is added to the remote service data required by the
 * module.
 */
void AbstractModule::addRemoteServiceData(const QString& string)
{
    _reqRemote.setServiceData(string);
}

/**
 * @details
 * The given \p list is added to the remote stream data required by the
 * module.
 */
void AbstractModule::addRemoteStreamData(const QSet<QString>& list)
{
    _reqRemote.addStreamData(list);
}

/**
 * @details
 * The given parameter is added to the remote stream data required by the
 * module.
 */
void AbstractModule::addRemoteStreamData(const QString& string)
{
    _reqRemote.setStreamData(string);
}

/**
 * @details
 * This method returns a combination of all the data (local and
 * remote) required by the module.
 */
const DataRequirements AbstractModule::requiredData() const
{
    return _reqRemote + _reqLocal;
}

/**
 * @details
 * Gets the configuration for the given \p tagName and \p attribute.
 */
QString AbstractModule::getOption(const QString& tagName, const QString& attribute)
{
    return _config.namedItem(tagName).toElement().attribute(attribute);
}

} // namespace pelican
