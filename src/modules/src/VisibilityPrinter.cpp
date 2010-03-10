#include "modules/VisibilityPrinter.h"
#include "utility/ConfigNode.h"
#include "data/VisibilityData.h"
#include <iostream>

#include "utility/memCheck.h"

namespace pelican {


/**
 * @details
 * VisibilityPrinter
 */
VisibilityPrinter::VisibilityPrinter(const ConfigNode& config)
    : AbstractModule(config)
{
}


/**
 * @details
 * Destructor
 */
VisibilityPrinter::~VisibilityPrinter()
{
}


/**
 * @details
 * Prints some visibilities.
 */
void VisibilityPrinter::run(VisibilityData* vis)
{
    std::cout << "Hello from VisibilityPrinter::run() method!\n";

    if (!vis)
        throw QString("Data blob missing: Visibility Data");

    if (vis->nAntennas() == 0)
        throw QString("Empty data blob: Visibility Data");

    unsigned nChan = vis->nChannels();
    unsigned nAnt = vis->nAntennas();
    unsigned nPol = vis->nPolarisations();
    for (unsigned p = 0; p < nPol; p++) {
        for (unsigned c = 0; c < nChan; c++) {
            for (unsigned j = 0; j < nAnt; j++) {
                for (unsigned i = 0; i < nAnt; i++) {
                    std::cout << "VisData: " << (*vis)(i, j, c, p) << std::endl;
                }
            }
        }
    }

}


} // namespace pelican
