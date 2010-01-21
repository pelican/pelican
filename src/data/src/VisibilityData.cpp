#include "data/VisibilityData.h"

namespace pelican {

/**
 * @details
 * Constructs a null set of visibility data.
 * The visibility matrix has zero size.
 */
VisibilityData::VisibilityData() : DataBlob()
{
}

/**
 * @details
 * Constructs an empty set of visibility data.
 * The visibility matrix is pre-sized to the given parameters.
 *
 * @param[in] rows The number of rows in the matrix.
 * @param[in] cols The number of columns in the matrix.
 */
VisibilityData::VisibilityData(int rows, int cols) : DataBlob()
{
}

/**
 * @details
 * Visibility data destructor.
 */
VisibilityData::~VisibilityData()
{
}

} // namespace pelican
