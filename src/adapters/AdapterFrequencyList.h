#ifndef ADAPTERFREQUENCYLIST_H
#define ADAPTERFREQUENCYLIST_H

#include "adapters/AbstractServiceAdapter.h"
#include "data/FrequencyList.h"
#include <QDataStream>

/**
 * @file AdapterFrequencyList.h
 */

namespace pelican {

class ConfigNode;

/**
 * @class AdapterFrequencyList
 *  
 * @brief
 * 
 * @details
 */

class AdapterFrequencyList : public AbstractServiceAdapter
{
    public:
        AdapterFrequencyList(const ConfigNode& config);
        ~AdapterFrequencyList();

    protected:
        /// Method to deserialise a frequency channel list file
        void deserialise(QDataStream& in);

    private:
        /// Sets frequency data blob being read into.
        void _setData();

    private:
        unsigned _nChan;
        unsigned _dataBytes;
        FrequencyList* _freqList;

};

} // namespace pelican

#endif // ADAPTERFREQUENCYLIST_H 
