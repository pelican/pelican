#ifndef VISIBILITYPRINTER_H
#define VISIBILITYPRINTER_H

#include "modules/AbstractModule.h"
#include "data/VisibilityData.h"
#include <QHash>
#include <QString>

/**
 * @file VisibilityPrinter.h
 */

namespace pelican {

class DataBlob;

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
        void run(QHash<QString, DataBlob*>& data);

    private:
        VisibilityData* _vis;
};

} // namespace pelican
#endif // VISIBILITYPRINTER_H
