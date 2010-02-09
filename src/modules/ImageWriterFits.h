#ifndef IMAGERWRITERFITS_H_
#define IMAGERWRITERFITS_H_

#include "AbstractModule.h"
#include "data/ImageData.h"

class QDomElement;

/**
 * @file ImageWriterFits.h
 */

namespace pelican {

/**
 * @class ImageWriterFits
 *  
 * @brief
 * Module which will produce a brightness map given a matrix of visibility data.
 * 
 * @details
 */

class ImageWriterFits : public AbstractModule
{
    public:
        /// Module constructor.
        ImageWriterFits(const QDomElement& config);

        /// Module destructor.
        ~ImageWriterFits();

        /// Runs the module.
        void run(QHash<QString, DataBlob*>& data);

    private:
        /// Extract the configuration from the xml node setting default where required.
        void _getConfiguration(const QDomElement& config);

    private:


};

} // namespace pelican

#endif // IMAGERFFT_H_
