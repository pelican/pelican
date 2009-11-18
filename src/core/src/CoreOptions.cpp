#include "CoreOptions.h"


namespace pelican {

/**
 * @details
 * Default CoreOptions constructor.
 */
CoreOptions::CoreOptions()
{
}

/**
 * @details
 * Overloaded CoreOptions constructor.
 */
CoreOptions::CoreOptions(int argc, char** argv)
{
}

/**
 * @details
 * CoreOptions destructor.
 */
CoreOptions::~CoreOptions()
{
}

/**
 * @details
 * Sets the type of data client required by the pipeline driver.
 */
void CoreOptions::setDataClient(const QString& type) 
{
    _dataClientType = type;
}

/**
 * @details
 * Returns the type of data client set by the pipeline driver.
 */
QString CoreOptions::dataClient() const
{
    return _dataClientType;
}


} // namespace pelican
