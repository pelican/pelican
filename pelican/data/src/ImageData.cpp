#include "pelican/data/ImageData.h"
#include <algorithm>
#include <QString>
#include <iomanip>
#include <iostream>
#include <cfloat>

namespace pelican {

PELICAN_DECLARE_DATABLOB(ImageData)

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
        const std::vector<unsigned>& channels, const Polarisation polarisation)
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
        const std::vector<unsigned>& channels, const Polarisation polarisation)
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
    _image.clear();
    _coordType = COORD_UNDEF;
    _ampUnits.clear();
    _sizeL = 0;
    _sizeM = 0;
    _nChannels = 0;
    _nPolarisations = 0;
    _polarisation = POL_UNDEF;
    _channels.clear();
    _refFreq = 0.0;
    _refChannel = 0;
    _deltaFreq = 0.0;
    _refCoordL = 0.0;
    _refPixelL = 0.0;
    _cellsizeL = 0.0;
    _refCoordM = 0.0;
    _refPixelM = 0.0;
    _cellsizeM = 0.0;
    _max.clear();
    _min.clear();
    _mean.clear();
}


/**
 * @details
 * Calculate the minimum and maximum pixel amplitude for the image plane
 * specified by channel \p c and polarisation \p p
 *
 * @param[in] c Channel.
 * @paran[in] p Polarisation.
 */
void ImageData::calculateAmplitudeRange(const unsigned c, const unsigned p)
{
    if (_image.empty())
        throw QString("ImageData::findAmpRange(): Image empty");

    const real_t *image = this->ptr(c, p);
    Q_ASSERT(image != NULL);
    unsigned index = c * _nPolarisations + p;
    _min[index] = DBL_MAX;
    _max[index] = -DBL_MAX;
    unsigned nPixels = _sizeL * _sizeM;
    for (unsigned i = 0; i < nPixels; i++) {
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
 * @paran[in] p Polarisation.
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
