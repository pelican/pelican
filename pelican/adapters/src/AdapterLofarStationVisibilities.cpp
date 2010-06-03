#include "pelican/adapters/AdapterLofarStationVisibilities.h"
#include "pelican/data/VisibilityData.h"
#include <QByteArray>
#include <QIODevice>
#include <iostream>
#include <iomanip>
#include <QStringList>

#include "pelican/utility/memCheck.h"

namespace pelican {

PELICAN_DECLARE_ADAPTER(AdapterLofarStationVisibilities)

/**
 * @details
 * Constructs a stream adapter for a LOFAR station visibility amplitude data set.
 * Extracts the default data blob dimensions from the default configuration
 * setting some defaults if configuration options can't be found.
 *
 * @param[in]   config  Pelican XML configuration node object.
 */
AdapterLofarStationVisibilities::AdapterLofarStationVisibilities(const ConfigNode& config)
: AbstractStreamAdapter(config)
{
    // Grab configuration for the adapter setting sensible defaults
    _nAnt = config.getOption("antennas", "number", "96").toUInt();
    QString rowMajor = config.getOption("rowMajor", "value", "true").toLower();
    _rowMajor = (rowMajor.startsWith("t")) ? true : false;
    _dataBytes = config.getOption("dataBytes", "number", "8").toUInt();
    _timeStart = config.getOption("time", "start", "0.0").toDouble();
    _timeDelta = config.getOption("time", "delta", "0.0").toDouble();

    // Get the channel and polarisation selection.
    _channels = config.getUnsignedList("channels");
    _polarisation = config.getPolarisation();
    _nPol = (_polarisation == POL_BOTH) ? 2 : 1;
    _nChan = _channels.size();
}

/**
 * @details
 */
AdapterLofarStationVisibilities::~AdapterLofarStationVisibilities()
{
}


/**
 * @details
 * Method for deserialise a LOFAR station visibility data set
 * contained in a QDataStream into a pelican visibility data blob.
 *
 * @param[in] in QIODevice containing a serialised version of a LOFAR
 *               visibility data set.
 */
void AdapterLofarStationVisibilities::deserialise(QIODevice* in)
{
    // Set local data pointers, resize data if needed, and perform sanity checks!
    _setData();

    // Read data from the stream
    complex_t* vis = _vis->ptr();

    // TODO Set the time stamp properly.
    _vis->setTimeStamp(_timeStart);

    unsigned nPointsPerPol = _nAnt * _nAnt * _nChan;
    unsigned nPointsPerChan = _nAnt * _nAnt;

    std::vector<char> temp(_chunkSize);
    in->read(&temp[0], _chunkSize);

    for (unsigned iRaw = 0, c = 0; c < _nChan; c++) {
        for (unsigned j = 0; j < _nAnt; j++) {
            for (unsigned pj = 0; pj < _nPol; pj++) {
                for (unsigned i = 0; i < _nAnt; i++) {
                    for (unsigned pi = 0; pi < _nPol; pi++) {

                        if (pi == pj) {
                            unsigned index = 0;
                            if (_rowMajor) {
                                index = pi * nPointsPerPol +
                                        c * nPointsPerChan + j * _nAnt + i;
                            }
                            else {
                                index = pi * nPointsPerPol +
                                        c * nPointsPerChan + i * _nAnt + j;
                            }
                            char *t = &temp[iRaw];
                            if (_dataBytes == 8)
                                vis[index] = *(reinterpret_cast< std::complex<double>* >(t));
                            else if (_dataBytes == 4)
                                vis[index] = *(reinterpret_cast< std::complex<float>* >(t));
                        }

                        iRaw+= 2 * _dataBytes;
                    }
                }
            }
        }
    }
}


/**
 * @details
 * Sets the size of the visibility data chunk which can be updated from
 * service data.
 * Checks the dimensions of the visibility data to be read matches the stream
 * chunk size.
 * Resizes the visibility data blob as required.
 */
void AdapterLofarStationVisibilities::_setData()
{
    if (_chunkSize == 0) {
        throw QString("No data to read. Stream chunk size set to zero.");
    }

    if (_data == NULL) {
        throw QString("Cannot deserialise into an unallocated blob!");
    }

    // If any service data exists update the visibility dimensions from it...
    if (!_serviceData.empty()) _updateDimensions();

    // Check the expected size due to data dimensions matches the stream chunk size
    unsigned dataSize = _nAnt * _nPol *_nAnt * _nChan * _nPol * _dataBytes * 2;
    if (dataSize != _chunkSize) {
        QString err = "AdapterLofarStationVisibilities::_setData() ";
        err += "Input data chunk size does not match expected data dimensions. ";
        err += "visibility blob size = " + QString::number(dataSize) + " != chunksize = " + QString::number(_chunkSize);
        throw err;
    }

    // Resize the visibility data being read into to match the adapter dimensions
    _vis = static_cast<VisibilityData*>(_data);

    std::vector<unsigned> channels(_nChan);
    for (unsigned i = 0; i < _nChan; i++) {
        channels[i] = i;
    }

    _vis->resize(_nAnt, channels, _polarisation);
}


/**
 * @details
 * Updates the visibility data dimensions from service data passed down
 * from the adapter configuration.
 */
void AdapterLofarStationVisibilities::_updateDimensions()
{
    // EXAMPLE ++ pseudo-code only +++
    // _nAnt = serviceData.nAnt;
    // _nChan = serviceData.nAnt;
    // _nPol = serviceData.nAnt; etc...
}


} // namespace pelican
