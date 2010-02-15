#include "AdapterFrequencyList.h"


#include "utility/memCheck.h"

namespace pelican {


/**
 * @details
 */
AdapterFrequencyList::AdapterFrequencyList(const ConfigNode& config)
    : AbstractServiceAdapter(config)
{
    _nChan = config.getOption("channels", "number", "512").toUInt();
    _dataBytes = config.getOption("dataBytes", "number", "8").toUInt();
}


/**
 * @details
 */
AdapterFrequencyList::~AdapterFrequencyList()
{
}


/**
 * @details
 */
void AdapterFrequencyList::deserialise(QDataStream& in)
{
    double* freq = _freqList->ptr();
    for (unsigned c = 0; c < _nChan; c++) {
        in >> freq[c];
    }
}


/**
 * @details
 */
void AdapterFrequencyList::_setData()
{
    if (_chunkSize == 0) {
        throw QString("No data to read. Stream chunk size set to zero.");
    }

    if (_data == NULL) {
        throw QString("Cannot deserialise into an unallocated blob!");
    }

    unsigned dataSize = _nChan * _dataBytes;
    if (dataSize != _chunkSize) {
        throw QString("Stream chunk size does not match data current dimensions");
    }

    _freqList = static_cast<FrequencyList*>(_data);
    _freqList->resize(_nChan);
}



} // namespace pelican
