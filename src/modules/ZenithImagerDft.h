#ifndef ZENITHIMAGERDFT_H
#define ZENITHIMAGERDFT_H

/**
 * @file ZenithImagerDft.h
 */

#include "modules/AbstractModule.h"
#include <vector>

namespace pelican {

class AbstractModule;
class AbstractAstrometry;
class ConfigNode;
class VisibilityData;
class ModelVisibilityData;
class AntennaPositions;
class ImageData;

/**
 * @class ZenithImagerDft
 *
 * @brief
 * Provides basic imaging routines for whole-sky views.
 *
 * @details
 */
class ZenithImagerDft : public AbstractModule
{
    private:
        friend class ZenithImagerDftTest;

    public:
        /// Module constructor.
        ZenithImagerDft(const ConfigNode& config);

        /// Module destructor.
        ~ZenithImagerDft();

        /// Sets the channels (\p c) produce an image from.
        /// Method overrides the selected channels from the configuration.
        void setChannels(const std::vector<unsigned>& c) { _channels = c; }

        /// Sets the polarisation selection.
        /// Method overrides the selected channels from the configuration.
        void setPolarisation(const unsigned p) { _polarisation = p; }

        /// Sets the image size.
        void setSize(const unsigned sizeL, const unsigned sizeM);

        /// Sets the image cellsize.
        void setCellsize(const double cellsizeL, const double cellsizeM);

        /// Sets the image size and cellsize.
        void setDimensions(const unsigned sizeL, const unsigned sizeM,
                const double cellsizeL, const double cellsizeM);

        /// Set the image size to the full sky.
        void setFullSky();

        /// Runs the module.
        void run(QHash<QString, DataBlob*>& data);

    private:
        /// Extract the configuration from the XML node setting default where required.
        void _getConfiguration(const ConfigNode& config);

        /// Generates an array of image coordinates in radians.
        void _calculateImageCoords(const double cellsize, const unsigned nPixels,
                real_t* coords);

        /// Fetches data blob pointers from the blob hash.
        void _fetchDataBlobs(QHash<QString, DataBlob*>& data);

        /// Calculate DFT weights for imaging.
        void _calculateWeights(const unsigned nAnt, real_t* antPos,
                const double frequency, const unsigned nCoords,
                real_t* imageCoord, complex_t* weights);

        /// Construct the image by DFT.
        void _makeImageDft(const unsigned nAnt, real_t* antPosX,
                real_t* antPosY, complex_t* vis, const double frequency,
                const unsigned nL, const unsigned nM, real_t* coordL,
                real_t* coordM, real_t *image);

        /// Element wise multiplication of two complex weights vectors.
        void _multWeights(const unsigned nAnt, complex_t* weightsXL,
                complex_t *weightsYM, complex_t *weights);

        /// Matrix vector multiply of visibilities by a weights vector.
        void _multMatrixVector(const unsigned nAnt, complex_t* visMatrix,
                complex_t *weights, complex_t *result);

        /// Vector-vector dot product taking the complex conjugate of the
        /// second vector.
        complex_t _vectorDotConj(const unsigned n, complex_t* a, complex_t* b);

        /// Cut the image outside unit radius in l, m.
        void _cutHemisphere(real_t* image, const unsigned nL, const unsigned nM,
                real_t *l, real_t *m);

        /// Sets the cellsize corresponding to a full sky image.
        void _setCellsizeFullSky();

        /// Remove auto-correlations.
        void _zeroAutoCorrelations(complex_t* vis, const unsigned nAnt);

        /// Generate a set of visibilities for creating a point spread function.
        void _setPsfVisibilties(complex_t* vis, const unsigned nAnt);

        /// Sets the image meta-data.
        void _setImageMetaData();

    private:
        enum { POL_X, POL_Y, POL_BOTH };
        enum { VIS_RAW, VIS_MODEL, VIS_CALIBRATED, VIS_PSF };

        VisibilityData* _vis;               ///< Visibility amplitude matrix.
        AntennaPositions* _antPos;          ///< Visibility positions matrix.
        ImageData* _image;                  ///< Image amplitude matrix.
        AbstractAstrometry* _astrometry;    ///< Astrometric conversion module.

        int _visUse;                        ///< Option to image raw, model or corrected.
        std::vector<unsigned> _channels;    ///< Selected channel list.
        unsigned _nChannels;
        int _freqRefChannel;                ///< Frequency reference channel
        double _freqRef;                    ///< Reference frequency
        double _freqDelta;                  ///< Frequency delta
        unsigned _polarisation;             ///< Selection polarisation (enumeration).
        bool _fullSky;                      ///< Image the full sky (overrides cellsize)
        bool _trimHemisphere;               ///< Cut the image to the full sky hemisphere
        bool _pixelCentred;                 ///< Put the zero of the image in the middle of a pixel
        unsigned _sizeL;                    ///< Image size in l (x) pixels.
        unsigned _sizeM;                    ///< Image size in m (y) pixels.
        double _cellsizeL;                  ///< Image pixel increment in m (y) direction.
        double _cellsizeM;                  ///< Image pixel increment in l (x) direction.

        std::vector<real_t> _coordL;        ///< Image l (x) coordinates in radians.
        std::vector<real_t> _coordM;        ///< Image m (y) coordinates in radians.

        std::vector<complex_t> _weightsXL;  ///< DFT weights for XL coordinates
        std::vector<complex_t> _weightsYM;  ///< DFT weights for YM coordinates
};

} // namespace pelican

#endif // ZENITHIMAGERDFT_H
