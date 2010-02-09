#ifndef IMAGERDFT_H_
#define IMAGERDFT_H_

#include "AbstractModule.h"
#include "data/ImageData.h"
#include "data/VisibilityData.h"
#include "data/VisibilityPositions.h"

class QDomElement;

/**
 * @file ImagerDft.h
 */

namespace pelican {

/**
 * @class ImagerDFT
 *  
 * @brief
 * Module which will produce a brightness map given a matrix of visibility data.
 * 
 * @details
 */

class ImagerDft : public AbstractModule
{
    public:
        /// Module constructor.
        ImagerDft(const QDomElement& config);

        /// Module destructor.
        ~ImagerDft();

        /// Sets the channel (\p c) which to produce an image from.
        /// Method overrides the selected channel from the configuration
        void setImageChannel(unsigned c) { _channel = c; }

        /// Runs the module.
        void run(QHash<QString, DataBlob*>& data);

    private:
        /// Extract the configuration from the xml node setting default where required.
        void _getConfiguration(const QDomElement& config);

        /// Generates an array of image coordinates in radians.
        void _calculateImageCoords(const double cellsize, const unsigned nPixels,
                real_t* coords);

        /// Calculate the an image weights vector for.
        void _calculateWeights(real_t* visCoord, const unsigned nVis,
                const double freqScale, real_t* imageCoord, const unsigned nCoords,
                complex_t* weights);

        /// Construct the image by DFT for polarisation (\p p) and channel (\p c)
        void _makeImageDftSimple(const unsigned nVis, const unsigned nL,
                const unsigned nM, complex_t* vis, complex_t* weightsUL,
                complex_t* weightsVM, real_t *image);

        /// Cut the image outside unit radius in l, m.
        void _cutHemisphere();

    private:
        VisibilityData *_vis;               ///< Visibility amplitude matrix.
        VisibilityPositions *_visCoord;     ///< Visibility positions matrix.
        ImageData *_image;                  ///< Image amplitude matrix.

        unsigned _channel;                  ///< Channel at which to generate the image
        unsigned _sizeL;                    ///< Image size in l (x) pixels.
        unsigned _sizeM;                    ///< Image size in m (y) pixels.
        unsigned _cellsizeL;                ///< Image pixel increment in m (y) direction.
        unsigned _cellsizeM;                ///< Image pixel increment in l (x) direction.

        std::vector<real_t> _coordL;        ///< Image l (x) coordinates in radians.
        std::vector<real_t> _coordM;        ///< Image m (y) coordinates in radians.

        std::vector<complex_t> _weightsUL;   ///< DFT weights \f$ w(u,l) exp^{(2 \pi i u l)} \f$
        std::vector<complex_t> _weightsVM;   ///< DFT weights \f$ w(u,l) exp^{(2 \pi i v m)} \f$
};

} // namespace pelican

#endif // IMAGERDFT_H_
