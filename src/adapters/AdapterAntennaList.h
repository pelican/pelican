#ifndef ADAPTERANTENNALIST_H
#define ADAPTERANTENNALIST_H

#include "adapters/AbstractServiceAdapter.h"
#include "data/AntennaPositions.h"
#include <QDataStream>

/**
 * @file AdapterAntennaList.h
 */

namespace pelican {

class ConfigNode;

/**
 * @class AdapterAntennaList
 *
 * @brief
 *
 * @details
 *
 */

class AdapterAntennaList : public AbstractServiceAdapter
{
    public:
        AdapterAntennaList(const ConfigNode& config);
        ~AdapterAntennaList();

    protected:
        /// Method to deserialise an antenna list file
        void deserialise(QIODevice* in);

    private:
        /// Sets antenna positions data blob being read into.
        void _setData();

    private:
        unsigned _nAnt;
        unsigned _dataBytes;
        AntennaPositions* _antPos;
};

} // namespace pelican

#endif // ADAPTERANTENNALIST_H
