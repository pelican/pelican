#ifndef IMAGERDFT_H_
#define IMAGERDFT_H_

#include "core/AbstractModule.h"

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

 */
class ImagerDFT : public AbstractModule
{
    public:
        /// Module constructor.
        ImagerDFT(const QDomElement& config);

        /// Module destructor.
        ~ImagerDFT();

        /// Runs the module.
        void run() {}

    private:
        void _generateExpTerms();
};

} // namespace pelican

#endif // IMAGERDFT_H_
