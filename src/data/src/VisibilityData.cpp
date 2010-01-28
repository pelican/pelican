#include "data/VisibilityData.h"

namespace pelican {

/**
 * @details
 * Constructs a null set of visibility data.
 * The visibility matrix has zero size.
 */
VisibilityData::VisibilityData() : DataBlob()
{
    _init();
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
    _init();
}

/**
 * @details
 * Visibility data destructor.
 */
VisibilityData::~VisibilityData()
{
}

/**
 * @details
 * Initialises the class data members (called from constructors).
 */
void VisibilityData::_init()
{
    _nAntennas = 0;
    _nChannels = 0;
}

} // namespace pelican
