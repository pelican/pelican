#include "modules/ImageCombiner.h"
#include "data/ImageData.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
ImageCombiner::ImageCombiner(const ConfigNode& config)
    : AbstractModule(config)
{
    // Get the module configuration.
    _getConfiguration(config);
}

/**
 * @details
 * Module destructor.
 */
ImageCombiner::~ImageCombiner()
{
}

/**
 * @details
 * Runs the image combiner module with the configured options.
 *
 * @param[in] in1 The first input image.
 * @param[in] in2 The second input image.
 * @param[out] out The output image.
 */
void ImageCombiner::run(const ImageData* in1, const ImageData* in2,
        ImageData* out)
{
    // Get the image dimensions.
    unsigned width = in1->sizeL();
    unsigned height = in1->sizeM();
    const std::vector<unsigned> channels = in1->channels();

    // Check that the image dimensions match.
    if (width != in2->sizeL() || height != in2->sizeM() || channels != in2->channels())
        throw QString("ImageCombiner::run(): Image dimension mismatch.");

    // Size the output image.
//    out->resize(width, height, );

    // Perform the required operation.
}

/**
 * @details
 * Performs image summation of two images \f$M_1\f$ and \f$M_2\f$,
 * using the expression:
 *
 * \f{equation}{
 *      a_1 M_1 + a_2 M_2 + a_3
 * \f}
 *
 * @param[in] width  The width of the images.
 * @param[in] height The height of the images.
 * @param[in] in1    The first input image.
 * @param[in] in2    The second input image.
 * @param[out] out   The output image.
 */
void ImageCombiner::_operationSum(unsigned width, unsigned height,
        const real_t* in1, const real_t* in2, real_t* out)
{
    for (unsigned h = 0; h < height; h++) {
        for (unsigned w = 0; w < width; w++) {
            unsigned index = w + h * width;
            out[index] = _a1 * in1[index] + _a2 * in2[index] + _a3;
        }
    }
}

/**
 * @details
 * Performs image summation of two images \f$M_1\f$ and \f$M_2\f$,
 * using the expression:
 *
 * \f{equation}{
 *      a_1 M_1 + a_2 M_2 + a_3
 * \f}
 *
 * Any NaN's (blanked pixels) images are replaced with 0.
 *
 * @param[in] width  The width of the images.
 * @param[in] height The height of the images.
 * @param[in] in1    The first input image.
 * @param[in] in2    The second input image.
 * @param[out] out   The output image.
 */
void ImageCombiner::_operationSumm(unsigned width, unsigned height,
        const real_t* in1, const real_t* in2, real_t* out)
{
    for (unsigned h = 0; h < height; h++) {
        for (unsigned w = 0; w < width; w++) {
            unsigned index = w + h * width;
            out[index] = _a1 * in1[index] + _a2 * in2[index] + _a3;
        }
    }
}

/**
 * @details
 * Gets and stores the module configuration options.
 */
void ImageCombiner::_getConfiguration(const ConfigNode& config)
{
    // Get the operation type.
    QString opcode = config.getOption("operation", "type", "").toLower();
    if (opcode == "sum")
        _opcode = SUM;
    else if (opcode == "summ")
        _opcode = SUMM;
    else if (opcode == "mean")
        _opcode = MEAN;
    else if (opcode == "mult")
        _opcode = MULT;
    else if (opcode == "div")
        _opcode = DIV;

    // Get the coefficients.
    bool ok = false;
    _a1 = config.getOption("coefficients", "a1", "0").toDouble(&ok);
    if (!ok) throw QString("ImageCombiner: Invalid coefficient a1.");
    _a2 = config.getOption("coefficients", "a2", "0").toDouble(&ok);
    if (!ok) throw QString("ImageCombiner: Invalid coefficient a2.");
    _a3 = config.getOption("coefficients", "a3", "0").toDouble(&ok);
    if (!ok) throw QString("ImageCombiner: Invalid coefficient a3.");
}

} // namespace pelican
