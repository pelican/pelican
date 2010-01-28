#include "data/ImageData.h"

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
ImageData::ImageData(int sizeL, int sizeM, int nChannels)
: DataBlob()
{
    assign(sizeL, sizeM, nChannels);
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
void ImageData::assign(int sizeL, int sizeM, int nChannels)
{
    _sizeL = sizeL;
    _sizeM = sizeM;
    _nChannels = nChannels;
    _image.resize(_nChannels * _sizeL * _sizeM);
}


/**
 * @details
 * Clears the image data blob.
 */
void ImageData::clear()
{
    _nChannels = 0;
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


} // namespace pelican
