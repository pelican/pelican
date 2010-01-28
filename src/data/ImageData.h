#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <vector>
#include <complex>

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
        const enum coordinates { COORD_RA_DEC, COORD_AZ_EL };

    public:
        /// Assign the image cube
        void assign(const int sizeL, const int sizeM, const int nChannels);

        /// Clears the image data blob
        void clear();

    public: // accessor methods

        /// Return the number of channels.
        int nChannels() const { return _nChannels; }

        /// Sets the number of channels
        void setNChannels(const unsigned int n) { _nChannels = n; }

        /// Returns the image size (number of pixels) in the L direction.
        int sizeL() const { return _sizeL; }

        /// Sets the image size in the L direction
        void setImageSizeL(const unsigned int size) { _sizeL = size; }

        /// Returns the image size (number of pixels) in the L direction.
        int sizeM() const { return _sizeM; }

        /// Sets the image size in the L direction
        void setImageSizeM(const unsigned int size) { _sizeM = size; }

        /// Returns the image pixel separation in the L direction in arcseconds.
        double cellsizeL() const { return _cellsizeL; }

        /// Sets the image pixel separation in the L direction
        void setCellsizeL(const double size) { _cellsizeL = size; }

        /// Returns the image pixel separation in the M direction arcseconds.
        double cellsizeM() const { return _cellsizeM; }

        /// Sets the image pixel separation in the M direction
        void setCellsizeM(const double size) { _cellsizeM = size; }

        /// Returns the reference pixel position in the L direction.
        double refPixelL() const { return _refPixelL; }

        /// Set the reference pixel in the L direction
        void setRefPixelL(const double pos) { _refPixelL = pos; }

        /// Returns the reference pixel in the M direction.
        double refPixelM() const { return _refPixelM; }

        /// Set the reference pixel in the L direction
        void setRefPixelM(const double pos) { _refPixelM = pos; }

        /// Returns the reference pixel coordinate type.
        int coordType() const { return _coordType; }

        /// Sets the reference pixel coordinate type.
        void setCoordType(const int type) { _coordType = type; }

        /// Returns the reference pixel coordinate in units specified by the coordinate type
        double refCoordL() const { return _refCoordL; }

        /// sets the reference pixel coordinate in units specified by the coordinate type
        void setRefCoordL(const double coord) { _refCoordL = coord; }

        /// Returns the reference pixel coordinate in units specified by the coordinate type
        double refCoordM() const { return _refCoordM; }

        /// sets the reference pixel coordinate in units specified by the coordinate type
        void setRefCoordM(const double coord) { _refCoordM = coord; }

        /// Returns the image pixel amplitude at the coordinate l,m for the specified channel
        real_t& at(const unsigned int l, const unsigned int m, const unsigned int channel) const {
            return _image[channel * _sizeL * _sizeM + m * _sizeL + l];
        }

        /// Return a pointer to the image cube.
        real_t* ampPtr() const { return _image.size() > 0 ? &_image[0] : NULL; }

        /// Return a pointer to the image for a specifed channel
        real_t* ampPtr(const unsigned int channel) const {
            return _image.size() > 0 ? &_image[channel * _sizeL * _sizeM] : NULL;
        }

        /// Returns a reference to the image amplitude vector
        std::vector<real_t>& amp() const { return _image; }

        /// Operator to dereference the image amplitude array
        real_t& operator[](const unsigned int i) const { return _image[i]; }

        /// Operator to dereference the image amplitude array
        real_t& operator()(const unsigned int l, const unsigned int m, const unsigned int channel) {
            return _image[channel * _sizeL * _sizeM + m * _sizeL + l];
        }

        /// Sets the image pixel amplitude at the coordinate l,m for the specified channel
        void setAmp(const unsigned int l, const unsigned int m, const unsigned int channel, const real_t value) {
            _image[channel * _sizeL * _sizeM + m * _sizeL + l] = value;
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
