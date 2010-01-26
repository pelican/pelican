#include "modules/BasicFlagger.h"

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
    addRemoteStreamData("Visibilities");

    /* Parse configuration and add extra data requirements as needed */
    if (getOption("use_flag_table", "value").toLower() == "true")
        addLocalServiceData("FlagTable");
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

}

} // namespace pelican
