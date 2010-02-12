#ifndef FILEADAPTERLOFARVISIBILITIES_H
#define FILEADAPTERLOFARVISIBILITIES_H

#include "adapters/AbstractAdapter.h"
#include "utility/ConfigNode.h"
#include <QDataStream>

/**
 * @file FileAdapterLofarVisibilities.h
 */

namespace pelican {

/**
 * @class FileAdapterLofarVisibilities
 *  
 * @brief
 * 
 * @details
 * 
 */

class FileAdapterLofarVisibilities : public AbstractAdapter
{
    public:
        FileAdapterLofarVisibilities(const ConfigNode& config);
        ~FileAdapterLofarVisibilities();

    public:
        void run(QDataStream& stream, const size_t& size) {};

    private:
};

} // namespace pelican

#endif // FILEADAPTERLOFARVISIBILITIES_H 
