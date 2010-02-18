#include "AdapterAntennaList.h"

#include "utility/memCheck.h"

namespace pelican {


// class AdapterAntennaList
AdapterAntennaList::AdapterAntennaList(const ConfigNode& config)
    : AbstractServiceAdapter(config)
{
    _nAnt = config.getOption("antennas", "number", "96").toUInt();
    _dataBytes = config.getOption("dataBytes", "number", "8").toUInt();
}


/**
 * @details
 */
AdapterAntennaList::~AdapterAntennaList()
{
}

/**
 * @details
 */
void AdapterAntennaList::deserialise(QIODevice* in)
{
//    real_t* x = _antPos->xPtr();
//    real_t* y = _antPos->xPtr();
//    real_t* z = _antPos->xPtr();
//    for (unsigned a = 0; a < _nAnt; a++) {
////        in >> x[a] >> y[a] >> z[a];
//    }
}



/**
 * @details
 */
void AdapterAntennaList::_setData()
{
//    if (_chunkSize == 0) {
//        throw QString("No data to read. Stream chunk size set to zero.");
//    }
//
//    if (_data == NULL) {
//        throw QString("Cannot deserialise into an unallocated blob!");
//    }
//
//    unsigned dataSize = _nAnt * 3 * _dataBytes;
//    if (dataSize != _chunkSize) {
//        throw QString("Stream chunk size does not match data current dimensions");
//    }
//
//    _antPos = static_cast<AntennaPositions*>(_data);
//    _antPos->resize(_nAnt);
}



} // namespace pelican
