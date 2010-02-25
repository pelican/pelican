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
 * @param[in]   sizeL       The image size (pixels) in the L direction.
 * @param[in]   sizeM       The image size (pixels) in the M direction.
 * @param[in]   nChannels   The number of frequency channels in the image cube.
 */
ImageData::ImageData(const unsigned sizeL, const unsigned sizeM,
        const unsigned nChannels, const unsigned nPolarisations)
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
void ImageData::resize(const unsigned sizeL, const unsigned sizeM,
        const unsigned nChannels, const unsigned nPolarisations)
{
    _ampUnits = "JY/PIXEL";
    _sizeL = sizeL;
    _sizeM = sizeM;
    _nChannels = nChannels;
    _nPolarisations = nPolarisations;
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
    _coordType = 0;
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

//    std::vector<long double> rowMean(_sizeM);
//
//    for (unsigned m = 0; m < _sizeM; m++) {
//        rowMean[m] = 0.0;
//        for (unsigned l = 0; l < _sizeL; l++) {
//            unsigned i = m * _sizeL + l;
//             rowMean[m] += image[i];
//        }
//        _mean[index] += rowMean[m] / _sizeL;
//    }
//    _mean[index] /= _sizeM;

//    std::vector<real_t> temp = _image;
//    std::sort(temp.begin(), temp.end(), _absSort);
//    for (unsigned i = 0; i < temp.size(); i++) {
//        _mean[index] += temp[i];
////        std::cout << std::scientific << std::setprecision(8);
////        std::cout << i << " " << temp[i] << " " << _mean[index] << std::endl;
//    }
//    _mean[index] /= temp.size();
}


/**
 * @details
 * Sort by abs value
 */
bool ImageData::_absSort(const real_t a, const real_t b)
{
    return std::abs(a) < std::abs(b);
}

} // namespace pelican
