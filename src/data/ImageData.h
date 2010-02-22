#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "data/DataBlob.h"
#include <vector>
#include <string>
#define isnan(x) ((x) != (x))

/**
 * @file ImageData.h
 */

namespace pelican {

/**
 * @class ImageData
 *
 * @brief
 * Image container class.
 *
 * @details
 * This class holds image data at a number of frequencies.
 */

class ImageData : public DataBlob
{
    public:
        /// Constructor
        ImageData();

        /// Constructor assigning memory for the image cube.
        ImageData(const unsigned& sizeL, const unsigned& sizeM,
                const unsigned& nChannels, const unsigned& nPolarisations);

        /// Image data destructor.
        ~ImageData();

    public:
        /// The coordinate type of the reference pixel
        enum { COORD_RA_DEC, COORD_AZ_EL };

    public:
        /// Assign the image cube
        void resize(const unsigned& sizeL, const unsigned& sizeM,
                const unsigned& nChannels, const unsigned& nPolarisations);

        /// Clears the image data blob
        void clear();

        /// Finds and sets the pixel amplitude range for the image data.
        void findAmpRange(const unsigned& c, const unsigned& p);

    public: // accessor methods

        // Returns the number of pixels in the image
        unsigned nPixels() const { return _image.size(); }

        /// Returns the number of channels.
        unsigned nChannels() const { return _nChannels; }

        /// Returns the number of polarisations
        unsigned nPolarisations() const { return _nPolarisations; }

        /// Returns the image size (number of pixels) in the L direction.
        unsigned sizeL() const { return _sizeL; }

        /// Returns the image size (number of pixels) in the L direction.
        unsigned sizeM() const { return _sizeM; }

        /// Returns a reference to the image pixel separation in the L direction in arcseconds.
        double& cellsizeL() { return _cellsizeL; }

        /// Returns a reference to the  image pixel separation in the M direction arcseconds.
        double& cellsizeM() { return _cellsizeM; }

        /// Returns a reference to the  reference pixel position in the L direction.
        double& refPixelL()  { return _refPixelL; }

        /// Returns a reference to the reference pixel in the M direction.
        double& refPixelM() { return _refPixelM; }

        /// Returns a reference to reference pixel coordinate type.
        int& coordType() { return _coordType; }

        /// Returns a reference to the reference pixel coordinate in units specified by the coordinate type
        double& refCoordL() { return _refCoordL; }

        /// Returns a reference to the reference pixel coordinate in units specified by the coordinate type
        double& refCoordM() { return _refCoordM; }

        /// Returns a reference to the image amplitude vector
        std::vector<real_t>& amp() { return _image; }

        /// Dereferences the image amplitude array for image index (\p i)
        real_t& operator[](const unsigned& i) { return _image[i]; }

        /// Dereferences the image amplitude array for image index (\p i)
        const real_t& operator[](const unsigned& i) const { return _image[i]; }

        /// Dereference the image amplitude array for image coordinate
        /// (\p l, \p m), channel (\p c) and polarisation (\p p)
        real_t& operator()(const unsigned& l, const unsigned& m,
                const unsigned& c, const unsigned& p) {
            unsigned index = l + _sizeL * (_sizeM * (p * _nChannels + c) + m);
            return _image[index];
        }

        /// Dereference the image amplitude array for image coordinate
        /// (\p l, \p m), channel (\p c) and polarisation (\p p)
        const real_t& operator()(const unsigned& l, const unsigned& m,
                const unsigned& c, const unsigned& p) const {
            unsigned index = l + _sizeL * (_sizeM * (p * _nChannels + c) + m);
            return _image[index];
        }

        /// Return a pointer to the image cube.
        real_t* ptr() { return _image.size() > 0 ? &_image[0] : NULL; }

        /// Return a pointer to the image for a specified polarisation (\p p)
        real_t* ptr(const unsigned& p) {
            unsigned index = p * _nChannels * _sizeL * _sizeM;
            return _image.size() > 0 ? &_image[index] : NULL;
        }

        /// Return a pointer to the image for a specified polarisation (\p p)
        /// and channel (\p c)
        real_t* ptr(const unsigned& p, const unsigned& c) {
            unsigned index = _sizeL * _sizeM * (p * _nChannels + c);
            return _image.size() > 0 ? &_image[index] : NULL;
        }

        /// Returns the maximum image pixel amplitude.
        const double& max(const unsigned& p, const unsigned& c) const  {
            unsigned index = c * _nPolarisations + p;
            return _max[index];
        }

        /// Returns the minimum image pixel amplitude.
        const double& min(const unsigned& p, const unsigned& c) const {
            unsigned index = c * _nPolarisations + p;
            return _max[index];
        }

        /// Returns the amplitude units
        std::string& ampUnits() { return _ampUnits; }

    private:
        std::vector<real_t> _image; ///< Image amplitude cube.
        unsigned _nChannels;        ///< Number of channels.
        unsigned _nPolarisations;   ///< Number of polarisations.
        unsigned _sizeL;            ///< Number of pixels along l (x).
        unsigned _sizeM;            ///< Number of pixels along m (y).
        double _cellsizeL;          ///< Pixel delta along l in arcseconds.
        double _cellsizeM;          ///< Pixel delta along m in arcseconds.
        double _refPixelL;          ///< Coordinate reference pixel for l axis.
        double _refPixelM;          ///< Coordinate reference pixel for m axis.
        int _coordType;             ///< Reference pixel coordinate type (enum)
        double _refCoordL;          ///< Reference coordinate in \p coordType units.
        double _refCoordM;          ///< Reference coordinate in \p coordType units.
        std::string _ampUnits;      ///< Amplitude unit.
        std::vector<double> _max;   ///< Maximum image pixel amplitude. (channel, polarisation)
        std::vector<double> _min;   ///< Minimum image pixel amplitude (channel, polarisation)
};

} // namespace pelican

#endif // IMAGEDATA_H
