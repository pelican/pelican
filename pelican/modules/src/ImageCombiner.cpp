#include "pelican/modules/ImageCombiner.h"
//#include <boost/math/special_functions/fpclassify.hpp>

#include "pelican/utility/memCheck.h"


namespace pelican {

PELICAN_DECLARE_MODULE(ImageCombiner)

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

    // Check that the image dimensions match.
    if (width != in2->sizeL() || height != in2->sizeM())
        throw QString("ImageCombiner::run(): Image dimension mismatch.");

    // Size the output image.
    out->resize(width, height, _channelsOutput, _polOutput);

    // Perform the required operation.
    for (unsigned p = 0; p < _nPolarisations; ++p) {
        for (unsigned c = 0; c < _nChannels; ++c) {
            unsigned c1 = findIndex(_channelsInput1[c], in1->channels());
            unsigned c2 = findIndex(_channelsInput2[c], in2->channels());
            unsigned p1 = 0, p2 = 0;
            if (_nPolarisations == 2) {
                p1 = p;
                p2 = p;
            } else {
                p1 = (_polInput1 == POL_X) ? 0 : 1;
                p2 = (_polInput2 == POL_X) ? 0 : 1;
            }

            const real_t* ptr1 = in1->ptr(c1, p1);
            const real_t* ptr2 = in2->ptr(c2, p2);
            real_t* ptr0 = out->ptr(c, p);

            switch (_opcode) {
            case SUM:
                _operationSum(width, height, ptr1, ptr2, ptr0);
                break;
            case SUMM:
                _operationSumm(width, height, ptr1, ptr2, ptr0);
                break;
            case MEAN:
                _operationMean(width, height, ptr1, ptr2, ptr0);
                break;
            case MULT:
                _operationMult(width, height, ptr1, ptr2, ptr0);
                break;
            case DIV:
                _operationDiv(width, height, ptr1, ptr2, ptr0);
                break;
            }
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
 * Any NaN's (blanked pixels) in images are replaced with 0.
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
            real_t i1 = isnan(in1[index]) ? 0 : in1[index];
            real_t i2 = isnan(in2[index]) ? 0 : in2[index];
            out[index] = _a1 * i1 + _a2 * i2 + _a3;
        }
    }
}

/**
 * @details
 * Performs an average (mean) of two images \f$M_1\f$ and \f$M_2\f$,
 * using the expression:
 *
 * \f{equation}{
 *      ( a_1 M_1 + a_2 M_2 ) / 2.0
 * \f}
 *
 * @param[in] width  The width of the images.
 * @param[in] height The height of the images.
 * @param[in] in1    The first input image.
 * @param[in] in2    The second input image.
 * @param[out] out   The output image.
 */
void ImageCombiner::_operationMean(unsigned width, unsigned height,
        const real_t* in1, const real_t* in2, real_t* out)
{
    for (unsigned h = 0; h < height; h++) {
        for (unsigned w = 0; w < width; w++) {
            unsigned index = w + h * width;
            out[index] = (_a1 * in1[index] + _a2 * in2[index]) / 2.0;
        }
    }
}

/**
 * @details
 * Performs a multiplication of two images \f$M_1\f$ and \f$M_2\f$,
 * using the expression:
 *
 * \f{equation}{
 *      a_1 M_1 M_2 + a_2
 * \f}
 *
 * @param[in] width  The width of the images.
 * @param[in] height The height of the images.
 * @param[in] in1    The first input image.
 * @param[in] in2    The second input image.
 * @param[out] out   The output image.
 */
void ImageCombiner::_operationMult(unsigned width, unsigned height,
        const real_t* in1, const real_t* in2, real_t* out)
{
    for (unsigned h = 0; h < height; h++) {
        for (unsigned w = 0; w < width; w++) {
            unsigned index = w + h * width;
            out[index] = _a1 * in1[index] * in2[index] + _a2;
        }
    }
}

/**
 * @details
 * Performs a division of two images \f$M_1\f$ and \f$M_2\f$,
 * using the expression:
 *
 * \f{equation}{
 *      a_1 M_1 / M_2 + a_2
 * \f}
 *
 * @param[in] width  The width of the images.
 * @param[in] height The height of the images.
 * @param[in] in1    The first input image.
 * @param[in] in2    The second input image.
 * @param[out] out   The output image.
 */
void ImageCombiner::_operationDiv(unsigned width, unsigned height,
        const real_t* in1, const real_t* in2, real_t* out)
{
    for (unsigned h = 0; h < height; h++) {
        for (unsigned w = 0; w < width; w++) {
            unsigned index = w + h * width;
            out[index] = _a1 * in1[index] / in2[index] + _a2;
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
    else
        throw QString("ImageCombiner: Unknown operation type.");

    // Get the coefficients.
    bool ok = false;
    _a1 = config.getOption("coefficients", "a1", "0").toDouble(&ok);
    if (!ok) throw QString("ImageCombiner: Invalid coefficient a1.");
    _a2 = config.getOption("coefficients", "a2", "0").toDouble(&ok);
    if (!ok) throw QString("ImageCombiner: Invalid coefficient a2.");
    _a3 = config.getOption("coefficients", "a3", "0").toDouble(&ok);
    if (!ok) throw QString("ImageCombiner: Invalid coefficient a3.");

    // Get the channels.
    _channelsInput1 = config.getUnsignedList("channelsInput1");
    _channelsInput2 = config.getUnsignedList("channelsInput2");
    _nChannels = _channelsInput1.size();
    if (_channelsInput1.size() != _channelsInput2.size())
        throw QString("ImageCombiner: Inconsistent channel mapping.");
    _channelsOutput.resize(_nChannels);
    for (unsigned i = 0; i < _nChannels; ++i) {
        _channelsOutput[i] = (_channelsInput1[i] == _channelsInput2[i]) ?
                _channelsInput1[i] : CHANNEL_UNDEF;
    }

    // Get the polarisation.
    _polInput1 = config.getPolarisation("polarisationInput1");
    _polInput2 = config.getPolarisation("polarisationInput2");
    _polOutput = config.getPolarisation("polarisationOutput");
    unsigned nPols1 = _polInput1 == POL_BOTH ? 2 : 1;
    unsigned nPols2 = _polInput2 == POL_BOTH ? 2 : 1;
    if (nPols1 != nPols2)
        throw QString("ImageCombiner: Inconsistent polarisation mapping.");
    _nPolarisations = _polOutput == POL_BOTH ? 2 : 1;
}

} // namespace pelican
