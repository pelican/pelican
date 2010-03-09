#include "data/ImageData.h"
#include <algorithm>
#include <QString>
#include <iomanip>
#include <iostream>

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
 * @param[in]   sizeL           Image size (pixels) in the L direction.
 * @param[in]   sizeM           Image size (pixels) in the M direction.
 * @param[in]   nChannels       Number of frequency channels in the image cube.
 * @param[in]   nPolarisations  Number of polarisations in the image cube.
 */
ImageData::ImageData(const unsigned sizeL, const unsigned sizeM,
        const std::vector<unsigned>& channels, const pol_t polarisation)
: DataBlob()
{
    clear();
    _ampUnits = "JY/PIXEL";
    resize(sizeL, sizeM, channels, polarisation);
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
void ImageData::resize(const unsigned sizeL, const unsigned sizeM,
        const std::vector<unsigned>& channels, const pol_t polarisation)
{
    _ampUnits = "JY/PIXEL";
    _sizeL = sizeL;
    _sizeM = sizeM;
    _channels = channels;
    _nChannels = channels.size();
    _polarisation = polarisation;
    _nPolarisations = (_polarisation == POL_BOTH) ? 2 : 1;
    _image.resize(_nPolarisations * _nChannels * _sizeL * _sizeM);
    _min.resize(_nPolarisations * _nChannels);
    _max.resize(_nPolarisations * _nChannels);
    _mean.resize(_nPolarisations * _nChannels);
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
    _coordType = COORD_UNDEF;
    _refCoordL = 0.0;
    _refCoordM = 0.0;
    _image.clear();
    _min.clear();
    _max.clear();
    _mean.clear();
}


/**
 * @details
 * Calculate the minimum and maximum pixel amplitude for the image plane
 * specified by channel \p c and polarisation \p p
 *
 * @param[in] c Channel.
 * @paran[in] p Polaristation.
 */
void ImageData::calculateAmplitudeRange(const unsigned c, const unsigned p)
{
    if (_image.empty())
        throw QString("ImageData::findAmpRange(): Image empty");

    const real_t *image = this->ptr(c, p);
    unsigned index = c * _nPolarisations + p;
    _min[index] = 1.0e99;
    _max[index] = -1.0e99;
    for (unsigned i = 0; i < _image.size(); i++) {
        if (isnan(image[i])) continue;
        _min[index] = std::min(image[i], _min[index]);
        _max[index] = std::max(image[i], _max[index]);
    }
}


/**
 * @details
 * Calculate the mean image pixel amplitude for the image plane
 * specified by channel \p c and polarisation \p p.
 *
 * Note: Needs to called before the cutting to the hemisphere to
 * return the expected zero mean result.
 *
 * @param[in] c Channel.
 * @paran[in] p Polaristation.
 */
void ImageData::calculateMean(const unsigned c, const unsigned p)
{
    if (_image.empty())
        throw QString("ImageData::findAmpRange(): Image empty");

    const real_t *image = this->ptr(c, p);
    unsigned index = c * _nPolarisations + p;
    _mean[index] = 0.0;
    for (unsigned i = 0; i < _image.size(); i++) {
        _mean[index] += image[i];
    }
    _mean[index] /= _image.size();
}


/**
 * @details
 * Sort comparison function for sorting by absolute value value.
 */
bool ImageData::_absSort(const real_t a, const real_t b)
{
    return std::abs(a) < std::abs(b);
}

} // namespace pelican
