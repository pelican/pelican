#ifndef VISIBILITYPRINTER_H
#define VISIBILITYPRINTER_H

#include "pelican/modules/AbstractModule.h"
#include <QHash>
#include <QString>

/**
 * @file VisibilityPrinter.h
 */

namespace pelican {

class DataBlob;
class VisibilityData;

/**
 * @class VisibilityPrinter
 *
 * @brief
 * Test module to print out visibilities
 *
 * @details
 *
 */

class VisibilityPrinter : public AbstractModule
{
    public:
        VisibilityPrinter(const ConfigNode& config);
        ~VisibilityPrinter();

    public:
        /// Runs the module
        void run(VisibilityData* vis);

    private:
        unsigned _counter;
};

} // namespace pelican
#endif // VISIBILITYPRINTER_H
