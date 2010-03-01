#include "modules/ZenithCalibrater.h"
#include "data/DataBlob.h"
#include "utility/ConfigNode.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
ZenithCalibrater::ZenithCalibrater(const ConfigNode& config)
    : AbstractModule(config)
{
}

/**
 * @details
 * Module destructor.
 */
ZenithCalibrater::~ZenithCalibrater()
{
}


/**
 * @detail
 */
void ZenithCalibrater::run(QHash<QString, DataBlob*>& data)
{

}


/**
 * @detail
 */
void ZenithCalibrater::_getConfiguration(const ConfigNode& config)
{
    _nEigenvalues = config.getOption("eigenvalues", "number", "10").toUInt();
    _tolerance = config.getOption("tolerance", "value", "1.0e-4").toDouble();

}


} // namespace pelican
