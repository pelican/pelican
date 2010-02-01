#include "modules/BasicFlagger.h"
#include "data/VisibilityData.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
BasicFlagger::BasicFlagger(const QDomElement& config)
    : AbstractModule(config)
{
    /* Set essential data requirements */
    addRemoteStreamData("VisibilityData");

    /* Parse configuration and add extra data requirements as needed */
    if (getOption("use_flag_table", "value").toLower() == "true") {
        _useFlagTable = true;
        addLocalServiceData("FlagTable");
    } else {
        _useFlagTable = false;
    }

    /* Get clipping levels */
    _minLevelMedian = getOption("min_level_median", "value", "0.25").toDouble();
    _maxLevelMedian = getOption("max_level_median", "value", "4.00").toDouble();
}

/**
 * @details
 * Module destructor.
 */
BasicFlagger::~BasicFlagger()
{
}

/**
 * @details
 * Runs the flagging module.
 *
 * @param[in,out] data The hash of data blobs required to run the pipeline
 *                     module. The module updates the contents of the data
 *                     blobs in the hash as required.
 */
void BasicFlagger::run(QHash<QString, DataBlob*>& data)
{
    /* Get a pointer to the visibility data */
    VisibilityData *visibilityData = static_cast<VisibilityData*>(data.value("VisibilityData"));
    if (!visibilityData)
        throw QString("No visibility data.");

    /* Make a local copy of the visibility data blob to find the median */
    VisibilityData localVis = *visibilityData;

}

} // namespace pelican
