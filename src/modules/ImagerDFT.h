#ifndef IMAGERDFT_H_
#define IMAGERDFT_H_

#include "AbstractModule.h"
#include "data/ImageData.h"
#include "data/VisibilityData.h"

class QDomElement;

/**
 * @file ImagerDFT.h
 */

namespace pelican {

/**
 * @class ImagerDFT
 *  
 * @brief
 * Module which will produce a brightness map given a matrix of visibility data.
 * 
 * @details
 *
 */
class ImagerDFT : public AbstractModule
{
    public:
        /// Module constructor.
        ImagerDFT(const QDomElement& config);

        /// Module destructor.
        ~ImagerDFT();

        /// Runs the module.
        void run(QHash<QString, DataBlob*>& data) {}

    private:
        /// Extract the configuration from the xml node setting default where required.
        void _getConfiguration(const QDomElement& config);

        /// Construct the image by dft
        void _dft();

    private:
        VisibilityData *_vis;
        ImageData *_image;
        unsigned int _sizeL;
        unsigned int _sizeM;
        unsigned int _cellsizeL;
        unsigned int _cellsizeM;
};

} // namespace pelican

#endif // IMAGERDFT_H_
