#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "data/DataBlob.h"
#include <vector>

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
        ImageData(const unsigned sizeL, const unsigned sizeM,
                const unsigned nChannels, const unsigned nPolarisations);

        /// Image data destructor.
        ~ImageData();

    public:
        /// The coordinate type of the reference pixel
        enum { COORD_RA_DEC, COORD_AZ_EL };

    public:
        /// Assign the image cube
        void assign(const unsigned sizeL, const unsigned sizeM,
                const unsigned nChannels, const unsigned nPolarisations);

        /// Clears the image data blob
        void clear();

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
        real_t& operator[](const unsigned i) { return _image[i]; }

        /// Dereferences the image amplitude array for image index (\p i)
        const real_t& operator[](const unsigned i) const { return _image[i]; }

        /// Dereference the image amplitude array for image coordinate
        /// (\p l, \p m), channel (\p c) and polarisation (\p p)
        real_t& operator()(const unsigned l, const unsigned m,
                const unsigned c, const unsigned p) {
            unsigned index = l + _sizeL * (_sizeM * (p * _nChannels + c) + m);
            return _image[index];
        }

        /// Dereference the image amplitude array for image coordinate
        /// (\p l, \p m), channel (\p c) and polarisation (\p p)
        const real_t& operator()(const unsigned l, const unsigned m,
                const unsigned c, const unsigned p) const {
            unsigned index = l + _sizeL * (_sizeM * (p * _nChannels + c) + m);
            return _image[index];
        }

        /// Return a pointer to the image cube.
        real_t* ptr() { return _image.size() > 0 ? &_image[0] : NULL; }

        /// Return a pointer to the image for a specified polarisation (\p p)
        real_t* ptr(const unsigned p) {
            unsigned index = p * _nChannels * _sizeL * _sizeM;
            return _image.size() > 0 ? &_image[index] : NULL;
        }

        /// Return a pointer to the image for a specified polarisation (\p p)
        /// and channel (\p c)
        real_t* ptr(const unsigned p, const unsigned c) {
            unsigned index = _sizeL * _sizeM * (p * _nChannels + c);
            return _image.size() > 0 ? &_image[index] : NULL;
        }


    private:
        /// Number of channels in the image cube
        unsigned _nChannels;
        /// Number of polarisations in the image cube
        unsigned _nPolarisations;
        /// Number of image pixels in the L/x direction.
        unsigned _sizeL;
        /// Number of image pixels in the M/y direction.
        unsigned _sizeM;
        /// Pixel separation in the L direction in arcseconds.
        double _cellsizeL;
        /// Pixel separation in the M direction in arcseconds.
        double _cellsizeM;
        /// Reference pixel location in the L direction.
        double _refPixelL;
        /// Reference pixel location in the M direction.
        double _refPixelM;
        /// Image reference pixel coordinate type. Values are that of the coordinates enum.
        int _coordType;
        /// Reference coordinate in enum coordType units.
        double _refCoordL;
        /// Reference coordinate in enum coordType units.
        double _refCoordM;
        /// Image amplitude cube.
        std::vector<real_t> _image;
};

} // namespace pelican

#endif // IMAGEDATA_H
