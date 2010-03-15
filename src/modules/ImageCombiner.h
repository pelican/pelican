#ifndef IMAGECOMBINER_H
#define IMAGECOMBINER_H

#include "AbstractModule.h"
#include "utility/ConfigNode.h"

/**
 * @file ImageCombiner.h
 */

namespace pelican {

class ImageData;

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

        /// Perform a summation of the two images.
        void _operationSum(unsigned width, unsigned height,
                const real_t* in1, const real_t* in2, real_t* out);

        /// Perform a summation of the two images.
        void _operationSumm(unsigned width, unsigned height,
                const real_t* in1, const real_t* in2, real_t* out);

    private:
        enum {SUM, SUMM, MEAN, MULT, DIV};
        int _opcode; ///< Type of image operation.

        double _a1; ///< First image coefficient.
        double _a2; ///< Second image coefficient.
        double _a3; ///< Third image coefficient.
};

} // namespace pelican

#endif // IMAGECOMBINER_H
