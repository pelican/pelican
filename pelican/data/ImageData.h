#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "pelican/data/DataBlob.h"
#include "pelican/utility/constants.h"
#include <QString>
#include <vector>
#include <cmath>
//#include <boost/math/special_functions/fpclassify.hpp>

/**
 * @file ImageData.h
 */

namespace pelican {

/**
 * @class ImageData
 *
 * @brief
 * Image data container class.
 *
 * @details
 * This class holds image data at a number of frequencies.
 */

class ImageData : public DataBlob
{
    public:
        /// The coordinate type of the reference pixel.
        typedef enum { COORD_J2000, COORD_AZ_EL, COORD_UNDEF } coord_t;

    private:
        std::vector<real_t> _image; ///< Image amplitude cube.

        coord_t _coordType;         ///< Reference pixel coordinate type (enum).
        QString _ampUnits;          ///< Amplitude unit.

        unsigned _sizeL;            ///< Number of pixels along l (x).
        unsigned _sizeM;            ///< Number of pixels along m (y).
        unsigned _nChannels;        ///< Number of channels in the image cube.
        unsigned _nPolarisations;   ///< Number of polarisations in the image cube.

        Polarisation _polarisation; ///< Image polarisation.
        std::vector<unsigned> _channels; ///< List of frequency channels in the image.

        double _refFreq;            ///< Reference frequency;
        double _refChannel;         ///< Channel of the reference frequency.
        double _deltaFreq;          ///< Frequency increment;

        double _refCoordL;          ///< Reference coordinate in \p _coordType units.
        double _refPixelL;          ///< Coordinate reference pixel for l axis.
        double _cellsizeL;          ///< Pixel delta along l in arcseconds.

        double _refCoordM;          ///< Reference coordinate in \p _coordType units.
        double _refPixelM;          ///< Coordinate reference pixel for m axis.
        double _cellsizeM;          ///< Pixel delta along m in arcseconds.

        std::vector<double> _max;   ///< Maximum image pixel amplitude (channel, polarisation).
        std::vector<double> _min;   ///< Minimum image pixel amplitude (channel, polarisation).
        std::vector<double> _mean;  ///< Mean image pixel amplitude (channel, polarisation).

    public:
        /// Constructor.
        ImageData();

        /// Constructor assigning memory for the image cube.
        ImageData(const unsigned sizeL, const unsigned sizeM,
                const std::vector<unsigned>& channels,
                const Polarisation polarisation);

        /// Image data destructor.
        ~ImageData();

    public:

        /// Resize the image cube.
        void resize(const unsigned sizeL, const unsigned sizeM,
                const std::vector<unsigned>& channels,
                const Polarisation polarisations);

        /// Clears the image data blob.
        void clear();

        /// Finds and sets the pixel amplitude range for the image data.
        void calculateAmplitudeRange(const unsigned c, const unsigned p);

        /// Finds and sets the pixel amplitude range for the image data.
        void calculateMean(const unsigned c, const unsigned p);

    public: // Accessor methods.

        /// Returns the number of pixels in the image.
        unsigned nPixels() const { return _image.size(); }

        /// Returns the number of channels in the image.
        unsigned nChannels() const { return _channels.size(); }

        /// Returns the vector of channels in the image.
        const std::vector<unsigned>& channels() const { return _channels; }

        /// Returns the reference frequency.
        double refFreq() const { return _refFreq; }

        /// Returns the reference frequency.
        double& refFreq() { return _refFreq; }

        /// Returns the channel of the reference frequency.
        double refChannel() const { return _refChannel; }

        /// Returns the channel of the reference frequency.
        double& refChannel() { return _refChannel; }

        /// Returns the frequency increment per channel.
        double deltaFreq() const { return _deltaFreq; }

        /// Returns the frequency increment per channel.
        double& deltaFreq() { return _deltaFreq; }

        /// Returns the number of polarisations.
        unsigned nPolarisations() const {
            return (_polarisation == POL_BOTH) ? 2 : 1;
        }

        /// Returns the polarisation enum value.
        Polarisation polarisation() const { return _polarisation; }

        /// Returns the image size (number of pixels) in the L direction.
        unsigned sizeL() const { return _sizeL; }

        /// Returns the image size (number of pixels) in the L direction.
        unsigned sizeM() const { return _sizeM; }

        /// Returns a reference to the image pixel separation at the reference
        /// pixel in the L direction in units of arcseconds.
        double& cellsizeL() { return _cellsizeL; }

        /// Returns a reference to the image pixel separation at the reference
        /// pixel in the M direction in units of arcseconds.
        double& cellsizeM() { return _cellsizeM; }

        /// Returns a reference to the reference pixel index in the L direction.
        double& refPixelL()  { return _refPixelL; }

        /// Returns a reference to the reference pixel index in the M direction.
        double& refPixelM() { return _refPixelM; }

        /// Returns a reference to reference pixel coordinate type (enum).
        coord_t& coordType() { return _coordType; }

        /// Returns a reference to the reference pixel coordinate in units
        /// specified by the coordinate type
        double& refCoordL() { return _refCoordL; }

        /// Returns a reference to the reference pixel coordinate in units
        /// specified by the coordinate type
        double& refCoordM() { return _refCoordM; }

        /// Returns the amplitude units string.
        QString& ampUnits() { return _ampUnits; }

        /// Returns the minimum image pixel amplitude.
        double min(const unsigned c, const unsigned p) const {
            unsigned index = c * _nPolarisations + p;
            return _min[index];
        }

        /// Returns the maximum image pixel amplitude.
        double max(const unsigned c, const unsigned p) const {
            unsigned index = c * _nPolarisations + p;
            return _max[index];
        }

        /// Returns the minimum image pixel amplitude.
        double mean(const unsigned c, const unsigned p) const {
            unsigned index = c * _nPolarisations + p;
            return _mean[index];
        }

    public: // Accessors to the image array

        /// Returns a reference to the image amplitude vector
        std::vector<real_t>& amp() { return _image; }

        /// Returns a reference to the image amplitude vector
        const std::vector<real_t>& amp() const { return _image; }

        /// Return a pointer to the image cube.
        real_t* ptr() { return _image.size() > 0 ? &_image[0] : NULL; }

        /// Return a pointer to the image cube.
        const real_t* ptr() const {
            return _image.size() > 0 ? &_image[0] : NULL;
        }

        /// Return a pointer to the image for a specified channel (\p c)
        real_t* ptr(const unsigned c) {
            unsigned index = c * _nPolarisations * _sizeL * _sizeM;
            return _image.size() > 0 ? &_image[index] : NULL;
        }

        /// Return a pointer to the image for a specified channel (\p c)
        const real_t* ptr(const unsigned c) const {
            unsigned index = c * _nPolarisations * _sizeL * _sizeM;
            return _image.size() > 0 ? &_image[index] : NULL;
        }

        /// Return a pointer to the image for a specified polarisation (\p p)
        /// and channel (\p c)
        real_t* ptr(const unsigned c, const unsigned p) {
            unsigned index = _sizeL * _sizeM * (c * _nPolarisations + p);
            return _image.size() > 0 ? &_image[index] : NULL;
        }

        /// Return a pointer to the image for a specified polarisation (\p p)
        /// and channel (\p c)
        const real_t* ptr(const unsigned c, const unsigned p) const {
            unsigned index = _sizeL * _sizeM * (c * _nPolarisations + p);
            return _image.size() > 0 ? &_image[index] : NULL;
        }

    public: // operators

        /// Dereferences the image amplitude array for image index (\p i)
        real_t& operator[] (const unsigned i) { return _image[i]; }

        /// Dereferences the image amplitude array for image index (\p i)
        real_t operator[] (const unsigned i) const { return _image[i]; }

        /// Dereference the image amplitude array for image coordinate
        /// (\p l, \p m), channel (\p c) and polarisation (\p p)
        real_t& operator() (const unsigned l, const unsigned m,
                const unsigned c, const unsigned p) {
            return _image[l + _sizeL * (_sizeM * (c * _nPolarisations + p) + m)];
        }

        /// Dereference the image amplitude array for image coordinate
        /// (\p l, \p m), channel (\p c) and polarisation (\p p)
        real_t operator() (const unsigned l, const unsigned m, const unsigned c,
                const unsigned p) const {
            return _image[l + _sizeL * (_sizeM * (c * _nPolarisations + p) + m)];
        }

    private:
        /// Comparison function for sorting by absolute value.
        static bool _absSort(const real_t a, const real_t b);
};

} // namespace pelican

#endif // IMAGEDATA_H
