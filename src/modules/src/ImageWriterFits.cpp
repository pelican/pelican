#include "modules/ImageWriterFits.h"
#include "utility/memCheck.h"
#include <iostream>

namespace pelican {

/**
 * @details
 * Module constructor.
 */
ImageWriterFits::ImageWriterFits(const QDomElement& config)
    : AbstractModule(config)
{
    // Register which data blobs are needed by the module
    addLocalStreamData("ImageData");

    // Extract configuration from the xml configuration node.
    _getConfiguration(config);
}


/**
 * @details
 * Module destructor.
 */
ImageWriterFits::~ImageWriterFits()
{
}


/**
 * @details
 * Method called by the pipeline to create images of the visibility data.
 */
void ImageWriterFits::run(QHash<QString, DataBlob*>& data)
{
}


/**
 * @details
 * Parse and extract options from the configuration xml node.
 */
void ImageWriterFits::_getConfiguration(const QDomElement &config)
{
}

} // namespace pelican
