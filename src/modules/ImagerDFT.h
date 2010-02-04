#ifndef IMAGERDFT_H_
#define IMAGERDFT_H_

#include "AbstractModule.h"
#include "data/ImageData.h"
#include "data/VisibilityData.h"
#include "data/VisibilityPositionsData.h"

class QDomElement;

/**
 * @file ImagerDFT.h
 */

namespace pelican {

/**
 * @class ImagerDFT
 *  
 * @brief
 * Module which will produce a brightness map given a matrix of visibility data.
 * 
 * @details
 *
 */
class ImagerDFT : public AbstractModule
{
    public:
        /// Module constructor.
        ImagerDFT(const QDomElement& config);

        /// Module destructor.
        ~ImagerDFT();

        /// Runs the module.
        void run(QHash<QString, DataBlob*>& data);

    private:
        /// Extract the configuration from the xml node setting default where required.
        void _getConfiguration(const QDomElement& config);

        /// Generates an array of image coordinates in radians.
        void _calculateImageCoords(const double cellsize, const unsigned nPixels,
                std::vector<real_t>& coords);

        /// Calculate the an image weights vector for.
        void _calculateImageWeights(unsigned channel,
                std::vector<real_t>& visCoord,
                std::vector<real_t>& imageCoord,
                std::vector<complex_t>& weight);

        /// Construct the image by DFT
        void _makeImageDft(unsigned channel);


        void _makeImageDft1(unsigned channel);

        /// Cut the image outside unit radius in l, m.
        void _cutHemisphere();

    private:
        VisibilityData *_vis;               ///< Visibility amplitude matrix.
        VisibilityPositions *_visCoord;     ///< Visibility positions matrix.
        ImageData *_image;                  ///< Image amplitude matrix.

        unsigned _sizeL;                    ///< Image size in l (x) pixels.
        unsigned _sizeM;                    ///< Image size in m (y) pixels.
        unsigned _cellsizeL;                ///< Image pixel increment in m (y) direction.
        unsigned _cellsizeM;                ///< Image pixel increment in l (x) direction.

        std::vector<real_t> _coordL;        ///< Image l (x) coordinates in radians.
        std::vector<real_t> _coordM;        ///< Image m (y) coordinates in radians.

        std::vector<complex_t> _weightUL;   ///< DFT weights \f$ w(u,l) exp^{(2 \pi i u l)} \f$
        std::vector<complex_t> _weightVM;   ///< DFT weights \f$ w(u,l) exp^{(2 \pi i v m)} \f$
};

} // namespace pelican

#endif // IMAGERDFT_H_
