#include "pelican/modules/VisibilityPrinter.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/data/VisibilityData.h"
#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

PELICAN_DECLARE_MODULE(VisibilityPrinter)

/**
 * @details
 * VisibilityPrinter
 */
VisibilityPrinter::VisibilityPrinter(const ConfigNode& config)
    : AbstractModule(config)
{
    _counter = 0;
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
    if (!vis)
        throw QString("Data blob missing: Visibility Data");

    if (vis->nAntennas() == 0)
        throw QString("Empty data blob: Visibility Data");


    std::cout << std::endl;
    std::cout << "VisibilityPrinter::run() Iteration " << _counter << std::endl;
    
    unsigned nChan = vis->nChannels();
    unsigned nAnt = vis->nAntennas();
    unsigned nPol = vis->nPolarisations();
    for (unsigned p = 0; p < nPol; p++) {
        for (unsigned c = 0; c < nChan; c++) {
            complex_t* ptr = vis->ptr(c, p);
            for (unsigned j = 0; j < nAnt; j++) {
                for (unsigned i = 0; i < nAnt; i++) {
                    std::cout << "VisibilityPrinter::run(): VisData: " << ptr[i + j * nAnt] << std::endl;
                }
            }
        }
    }
    _counter++;
}


} // namespace pelican
