#include "data/ImageData.h"
#include <algorithm>

namespace pelican {

/**
 * @details
 * Constructs an empty image data blob.
 */
ImageData::ImageData() : DataBlob()
{
}


/**
 * @details
 * Constructs and assigns memory for an image data blob.
 *
 * @param[in]   sizeL       The image size (pixels) in the L direction.
 * @param[in]   sizeM       The image size (pixels) in the M direction.
 * @param[in]   nChannels   The number of frequency channels in the image cube.
 */
ImageData::ImageData(const unsigned sizeL, const unsigned sizeM,
        const unsigned nChannels, const unsigned nPolarisations)
: DataBlob()
{
    assign(sizeL, sizeM, nChannels, nPolarisations);
}


/**
 * @details
 * Image data destructor.
 */
ImageData::~ImageData()
{
}


/**
 * @details
 * Assigns memory for an image data blob.
 *
 * @param[in]   sizeL       The image size (pixels) in the L direction.
 * @param[in]   sizeM       The image size (pixels) in the M direction.
 * @param[in]   nChannels   The number of frequency channels in the image cube.
 */
void ImageData::assign(const unsigned sizeL, const unsigned sizeM,
        const unsigned nChannels, const unsigned nPolarisations)
{
    _sizeL = sizeL;
    _sizeM = sizeM;
    _nChannels = nChannels;
    _nPolarisations = nPolarisations;
    _image.resize(_nPolarisations * _nChannels * _sizeL * _sizeM);
}


/**
 * @details
 * Clears the image data blob.
 */
void ImageData::clear()
{
    _nChannels = 0;
    _nPolarisations = 0;
    _sizeL = 0;
    _sizeM = 0;
    _cellsizeL = 0.0;
    _cellsizeM = 0.0;
    _refPixelL = 0.0;
    _refPixelM = 0.0;
    _coordType = 0;
    _refCoordL = 0.0;
    _refCoordM = 0.0;
    _image.clear();
}


/**
 * @details
 */
void ImageData::findAmpRange()
{
    if (_image.empty()) return;
    real_t *image = &_image[0];
    _ampMin = 1.0e99;
    _ampMax = -1.0e99;
    for (unsigned i = 0; i < _image.size(); i++) {
        _ampMin = std::min(image[i], _ampMin);
        _ampMax = std::max(image[i], _ampMax);
    }
}

} // namespace pelican
