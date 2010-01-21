#ifndef VISIBILITYDATA_H
#define VISIBILITYDATA_H

#include "data/DataBlob.h"
#include <vector>

/**
 * @file VisibilityData.h
 */

namespace pelican {

/**
 * @class VisibilityData
 *  
 * @brief
 * Holds visibility data.
 * 
 * @details
 * This class holds telescope visiblity data. It inherits
 * the DataBlob base class.
 */
class VisibilityData : public DataBlob
{
    public: /* Data (public for higher performance) */
        /// The visibility data.
        // Use matrix from Armadillo (http://arma.sourceforge.net) here?

    public:
        /// Constructs an empty visibility matrix.
        VisibilityData();

        /// Constructs a pre-sized visibility matrix.
        VisibilityData(int rows, int cols);

        /// Visibility data destructor.
        ~VisibilityData();
};

} // namespace pelican

#endif // VISIBILITYDATA_H
