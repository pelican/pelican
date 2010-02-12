#ifndef IMAGERFFT_H_
#define IMAGERFFT_H_

#include "AbstractModule.h"
#include "data/ImageData.h"
#include "data/VisibilityData.h"
#include "data/VisibilityPositions.h"
#include "utility/ConfigNode.h"

/**
 * @file ImagerFft.h
 */

namespace pelican {

/**
 * @class ImagerFft
 *  
 * @brief
 * Module which will produce a brightness map given a matrix of visibility data.
 * 
 * @details
 */

class ImagerFft : public AbstractModule
{
    public:
        /// Module constructor.
        ImagerFft(const ConfigNode& config);

        /// Module destructor.
        ~ImagerFft();

        /// Runs the module.
        void run(QHash<QString, DataBlob*>& data);

    private:
        /// Extract the configuration from the XML node setting default where required.
        void _getConfiguration(const ConfigNode& config);

    private:
        VisibilityData *_vis;           ///< Visibility amplitude matrix.
        VisibilityPositions *_visCoord; ///< Visibility positions matrix.
        ImageData *_image;              ///< Image amplitude matrix.

        unsigned _sizeL;                ///< Image size in l (x) pixels.
        unsigned _sizeM;                ///< Image size in m (y) pixels.
        unsigned _cellsizeL;            ///< Image pixel increment in m (y) direction.
        unsigned _cellsizeM;            ///< Image pixel increment in l (x) direction.
};

} // namespace pelican

#endif // IMAGERFFT_H_
