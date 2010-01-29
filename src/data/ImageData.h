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
        ImageData(const unsigned int sizeL, const unsigned int sizeM, const unsigned int nChannels);

        /// Image data destructor.
        ~ImageData();

    public:
        /// The coordinate type of the reference pixel
        enum { COORD_RA_DEC, COORD_AZ_EL };

    public:
        /// Assign the image cube
        void assign(const unsigned int sizeL, const unsigned int sizeM, const unsigned int nChannels);

        /// Clears the image data blob
        void clear();

    public: // accessor methods

        // Returns the number of pixels in the image
        int nPixels() const { return _image.size(); }

        /// Returns the number of channels.
        int nChannels() const { return _nChannels; }

        /// Returns the image size (number of pixels) in the L direction.
        int sizeL() const { return _sizeL; }

        /// Returns the image size (number of pixels) in the L direction.
        int sizeM() const { return _sizeM; }

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

        /// Returns a reference to the image pixel amplitude at the coordinate l,m for the specified channel
        real_t& amp(const unsigned int l, const unsigned int m, const unsigned int channel) {
            return _image[l + _sizeL * (m + _sizeM * channel)];
        }

        /// Returns a reference to the image amplitude vector
        std::vector<real_t>& amp() { return _image; }

        /// Operator to dereference the image amplitude array
        real_t& operator[](const unsigned int i) { return _image[i]; }

        /// Operator to dereference the image amplitude array
        real_t& operator()(const unsigned int l, const unsigned int m, const unsigned int channel) {
            return _image[l + _sizeL * (m + _sizeM * channel)];
        }

        /// Return a pointer to the image cube.
        real_t* ampPtr() { return _image.size() > 0 ? &_image[0] : NULL; }

        /// Return a pointer to the image for a specifed channel
        real_t* ampPtr(const unsigned int channel) {
            return _image.size() > 0 ? &_image[channel * _sizeL * _sizeM] : NULL;
        }

    private:
        /// Number of channels in the image cube
        int _nChannels;
        /// Number of image pixels in the L/x direction.
        int _sizeL;
        /// Number of image pixels in the M/y direction.
        int _sizeM;
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
