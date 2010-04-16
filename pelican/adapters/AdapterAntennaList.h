#ifndef ADAPTERANTENNALIST_H
#define ADAPTERANTENNALIST_H

#include "pelican/adapters/AbstractServiceAdapter.h"
#include <QDataStream>

/**
 * @file AdapterAntennaList.h
 */

namespace pelican {

class ConfigNode;
class AntennaPositions;

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
        /// Constructs a new AdapterAntennaList object.
        AdapterAntennaList(const ConfigNode& config);

        /// Method to deserialise an antenna list file.
        void deserialise(QIODevice* in);

    private:
        /// Sets antenna positions data blob being read into.
        void _setData();

    private:
        unsigned _nAnt;
        AntennaPositions* _antPos;
};

} // namespace pelican

#endif // ADAPTERANTENNALIST_H
