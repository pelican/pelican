#ifndef FILEADAPTERLOFARVISIBILITIES_H
#define FILEADAPTERLOFARVISIBILITIES_H

#include "adapters/AbstractAdapter.h"
#include "data/VisibilityData.h"
#include <QDataStream>

/**
 * @file FileAdapterLofarVisibilities.h
 */

namespace pelican {

class ConfigNode;

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
        /// Constructs the adapter
        FileAdapterLofarVisibilities(const ConfigNode& config);

        /// Destorys the adapter
        ~FileAdapterLofarVisibilities();

        /// Sets the data blob the adapter is going to deseralise into.
        void setData(VisibilityData &data) { _data = &data; }

    public:
        /// The number of antenna in the data stream chunk.
        unsigned& nAntenna() { return _nAnt; }

        /// The number of channels in the data stream chunk.
        unsigned& nChannels() { return _nChan; }

        /// The number of polarisations in the data stream chunk.
        unsigned& nPolarisations() { return _nPol; }

        /// Returns the number of bytes per data point in the chunk.
        unsigned& dataBytes() { return _dataBytes; }

    protected:
        /// Method to deseralise a LOFAR visibility file data stream.
        virtual void deseralise(QDataStream& in) const;

    private:
        VisibilityData *_data;  ///< Pointer to visibility data being populated

        unsigned _nAnt;         ///< Number of antennas in the chunk
        unsigned _nChan;        ///< Number of channels in the chunk
        unsigned _nPol;         ///< Number of polarisations in the chunk
        unsigned _dataBytes;    ///< number of bytes per data point
};

} // namespace pelican

#endif // FILEADAPTERLOFARVISIBILITIES_H 
