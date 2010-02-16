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
    addStreamData("VisibilityData");
    _vis = NULL;
}



VisibilityPrinter::~VisibilityPrinter()
{
}

void VisibilityPrinter::run(QHash<QString, DataBlob*>& data)
{
//    _vis = static_cast<VisibilityData*>(data["VisibilityData"]);
//    if (!_vis) throw QString("Data blob missing: Visibility Data");
//    if (_vis->nAntennas() == 0) throw QString("Empty data blob: Visibility Data");

//    complex_t* vis = _vis->ptr();
//    unsigned nChan = _vis->nChannels();
//    unsigned nAnt = _vis->nAntennas();
//    unsigned nPol = _vis->nPolarisations();
//    for (unsigned p = 0; p < nPol; p++) {
//        for (unsigned c = 0; c < nChan; c++) {
//            for (unsigned j = 0; j < nAnt; j++) {
//                for (unsigned i = 0; i < nAnt; i++) {
//            }
//        }
//    }
    std::cout << "Hello from VisibilityPrinter::run() method!\n";
}


} // namespace pelican
