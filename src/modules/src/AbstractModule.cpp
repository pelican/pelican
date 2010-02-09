#include "modules/AbstractModule.h"

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
 * If the \p tagName and/or \p attribute do not exist, \p defValue is returned.
 */
QString AbstractModule::getOption(const QString& tagName,
        const QString& attribute, const QString& defValue)
{
    return _config.namedItem(tagName).toElement().attribute(attribute,
            defValue);
}


/**
 * @details
 * Gets the configuration text for the given \p tagName.
 * If the \p tagName and/or text do not exist, \p defValue is returned.
 */
QString AbstractModule::getOptionText(const QString& tagName,
        const QString& defValue)
{
    QDomNode node = _config.namedItem(tagName);
    if (node.isNull()) return defValue;

    QDomNodeList children = node.childNodes();
    QString text = QString();

    if (children.isEmpty()) return defValue;
    else {
       for (int c = 0; c < children.size(); c++) {
           if (children.at(c).nodeType() == QDomNode::TextNode) {
               text += children.at(c).nodeValue();
           }
       }
    }
    return text.isEmpty() ? defValue : text;
}


} // namespace pelican
