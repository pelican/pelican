#include "data/ImageData.h"
#include <algorithm>

namespace pelican {

/**
 * @details
 * Constructs an empty image data blob.
 */
ImageData::ImageData() : DataBlob()
{
    clear();
}


/**
 * @details
 * Constructs and assigns memory for an image data blob.
 *
 * @param[in]   sizeL       The image size (pixels) in the L direction.
 * @param[in]   sizeM       The image size (pixels) in the M direction.
 * @param[in]   nChannels   The number of frequency channels in the image cube.
 */
ImageData::ImageData(const unsigned& sizeL, const unsigned& sizeM,
        const unsigned& nChannels, const unsigned& nPolarisations)
: DataBlob()
{
    clear();
    _ampUnits = "JY/PIXEL";
    resize(sizeL, sizeM, nChannels, nPolarisations);
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
 * Resizes the memory for an image data blob.
 *
 * @param[in] sizeL             Image size (pixels) in the L direction.
 * @param[in] sizeM             Image size (pixels) in the M direction.
 * @param[in] nChannels         Number of frequency channels in the image cube.
 * @param[in] nPolaristaions    Number of polarisations in the image cube.
 */
void ImageData::resize(const unsigned& sizeL, const unsigned& sizeM,
        const unsigned& nChannels, const unsigned& nPolarisations)
{
    _sizeL = sizeL;
    _sizeM = sizeM;
    _nChannels = nChannels;
    _nPolarisations = nPolarisations;
    _ampUnits = "JY/PIXEL";
    _image.resize(_nPolarisations * _nChannels * _sizeL * _sizeM);
    _min.resize(_nPolarisations * _nChannels);
    _max.resize(_nPolarisations * _nChannels);
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
void ImageData::findAmpRange(const unsigned& c, const unsigned& p)
{
    if (_image.empty()) return;
    real_t *image = this->ptr(p, c);
    unsigned index = c * _nPolarisations + p;
    _min[index] = 1.0e99;
    _max[index] = -1.0e99;
    for (unsigned i = 0; i < _image.size(); i++) {
        if (isnan(image[i])) continue;
        _min[index] = std::min(image[i], _min[index]);
        _max[index] = std::max(image[i], _max[index]);
    }
}

} // namespace pelican
