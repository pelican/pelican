#ifndef VISIBILITYDATA_H
#define VISIBILITYDATA_H

#include "data/DataBlob.h"
#include <vector>

/**
 * @file VisibilityData.h
 */

namespace pelican {

/**
 * @class VisibilityData
 *  
 * @brief
 * Holds visibility data.
 * 
 * @details
 * This class holds telescope visibility data. It inherits
 * the DataBlob base class.
 */
class VisibilityData : public DataBlob
{
    private: /* Data */
        /// The visibility data.
        /// Internally, the visibility data is stored as a contiguous memory
        /// block wrapped inside a standard vector.
        /// The data is indexed by antenna (i, j) and frequency channel (c).
        std::vector<complex_t> _visibilities;

        /// The number of antennas.
        int _nAntennas;

        /// The number of frequency channels.
        int _nChannels;

    public:
        /// Constructs an empty visibility cube.
        VisibilityData();

        /// Constructs a pre-sized visibility cube.
        VisibilityData(int antennas, int channels);

        /// Visibility data destructor.
        ~VisibilityData();

        /// Returns the number of antennas.
        int nAntennas() {return _nAntennas;}

        /// Returns the number of frequency channels.
        int nChannels() {return _nChannels;}

        /// Returns a pointer to the first element of the memory block.
        complex_t* memptr() {
            return _visibilities.size() > 0 ? &_visibilities[0] : NULL;
        }

    private: /* Methods */
        /// Initialises the data members.
        void _init();
};

} // namespace pelican

#endif // VISIBILITYDATA_H
