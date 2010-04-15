#include <QStringList>
#include <QtGlobal>
#include "pelican/data/DataBlobFactory.h"

// All data blob headers.
#include "pelican/data/AntennaPositions.h"
#include "pelican/data/DataBlob.h"
#include "pelican/data/FlagTable.h"
#include "pelican/data/FrequencyList.h"
#include "pelican/data/ImageData.h"
#include "pelican/data/VisibilityData.h"
#include "pelican/data/VisibilityPositions.h"
#include "pelican/data/ModelVisibilityData.h"
#include "pelican/data/CorrectedVisibilityData.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * DataBlobFactory constructor.
 */
DataBlobFactory::DataBlobFactory()
{
}

/**
 * @details
 * DataBlobFactory destructor.
 */
DataBlobFactory::~DataBlobFactory()
{
}

/**
 * @details
 * Public interface to create a new data blob. This always creates a data
 * blob, if it can. The data blob is <b>NOT</b> owned by the factory, and
 * must be deleted elsewhere.
 *
 * The known data blobs are:
 * - AntennaGains
 * - AntennaPositions
 * - FlagTable
 * - FrequencyList
 * - ImageData
 * - VisibilityData
 * - VisibilityPositions
 * - ModelVisibilityData
 * - CorrectedVisibilityData
 *
 * @param[in] type A string containing the type of the data blob.
 *
 * @return
 * Returns a pointer to the newly-created data blob.
 */
DataBlob* DataBlobFactory::create(const QString& type)
{
    DataBlob* dataBlob = NULL;

    /* Check which type of blob to create */
    if (type == "AntennaPositions")
        dataBlob = new AntennaPositions;
    else if (type == "DataBlob")
        dataBlob = new DataBlob;
    else if (type == "FlagTable")
        dataBlob = new FlagTable;
    else if (type == "FrequencyList")
        dataBlob = new FrequencyList;
    else if (type == "ImageData")
        dataBlob = new ImageData;
    else if (type == "VisibilityData")
        dataBlob = new VisibilityData;
    else if (type == "VisibilityPositions")
        dataBlob = new VisibilityPositions;
    else if (type == "ModelVisibilityData")
        dataBlob = new ModelVisibilityData;
    else if (type == "CorrectedVisibilityData")
        dataBlob = new CorrectedVisibilityData;

    /* Return pointer to the blob */
    return dataBlob;
}

} // namespace pelican
