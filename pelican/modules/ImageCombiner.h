#ifndef IMAGECOMBINER_H
#define IMAGECOMBINER_H

#include "pelican/modules/AbstractModule.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/constants.h"
#include "pelican/data/ImageData.h"

/**
 * @file ImageCombiner.h
 */

namespace pelican {

class ConfigNode;

/**
 * @class ImageCombiner
 *
 * @brief
 * Combines two images in various ways.
 *
 * @details
 * This module combines two images in various ways.
 * It can be used to generate difference maps.
 */
class ImageCombiner : public AbstractModule
{
    public:
        /// Module constructor.
        ImageCombiner(const ConfigNode& config);

        /// Module destructor.
        ~ImageCombiner();

        /// Runs the module.
        void run(const ImageData* in1, const ImageData* in2, ImageData* out);

    private:
        /// Grab configuration options from the config node.
        void _getConfiguration(const ConfigNode& config);

        /// Compute the sum of the two images.
        void _operationSum(unsigned width, unsigned height,
                const real_t* in1, const real_t* in2, real_t* out);

        /// Compute the sum of the two images.
        void _operationSumm(unsigned width, unsigned height,
                const real_t* in1, const real_t* in2, real_t* out);

        /// Compute the mean of the two images.
        void _operationMean(unsigned width, unsigned height,
                const real_t* in1, const real_t* in2, real_t* out);

        /// Multiply the two images.
        void _operationMult(unsigned width, unsigned height,
                const real_t* in1, const real_t* in2, real_t* out);

        /// Divide the two images.
        void _operationDiv(unsigned width, unsigned height,
                const real_t* in1, const real_t* in2, real_t* out);

    private:
        enum {SUM, SUMM, MEAN, MULT, DIV};
        static const unsigned CHANNEL_UNDEF = 100000;
        int _opcode; ///< Type of image operation.

        real_t _a1; ///< First image coefficient.
        real_t _a2; ///< Second image coefficient.
        real_t _a3; ///< Third image coefficient.
        unsigned _nChannels;      ///< Number of channels to output.
        unsigned _nPolarisations; ///< Number of polarisations to output.
        std::vector<unsigned> _channelsInput1;
        std::vector<unsigned> _channelsInput2;
        std::vector<unsigned> _channelsOutput;
        Polarisation _polInput1;
        Polarisation _polInput2;
        Polarisation _polOutput;
};

} // namespace pelican

#endif // IMAGECOMBINER_H
