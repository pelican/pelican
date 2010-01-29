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
 * Holds visibility snapshot data.
 * 
 * @details
 * This class holds telescope visibility data. It inherits
 * the DataBlob base class.
 *
 * Internally, the visibility data is stored as a contiguous memory
 * block wrapped inside a standard vector.
 *
 * A data cube consists of square matrices of complex correlation
 * products, indexed by antenna (i, j), and stacked in planes of
 * independent frequency channels (c).
 *
 * The data for each polarisation (p) is stored in separate, but contiguous,
 * data cubes.
 *
 * The matrix data is stored in column-major order for compatibility
 * with FORTRAN libraries like LAPACK.
 */
class VisibilityData : public DataBlob
{
    private: /* Data */
        /// The visibility data.
        std::vector<complex_t> _visibilities;

        /// The number of antennas.
        int _nAntennas;

        /// The number of frequency channels.
        int _nChannels;

        /// The number of polarisations.
        int _nPolarisations;

    public:
        /// Constructs an empty visibility cube.
        VisibilityData();

        /// Constructs a pre-sized visibility cube.
        VisibilityData(int antennas, int channels, int polarisations);

        /// Visibility data destructor.
        ~VisibilityData();

        /// Returns the number of antennas.
        int nAntennas() const {return _nAntennas;}

        /// Returns the number of frequency channels.
        int nChannels() const {return _nChannels;}

        /// Returns the number of polarisations.
        int nPolarisations() const {return _nPolarisations;}

        /// Returns a pointer to the first element of the memory block.
        complex_t* ptr() {
            return _visibilities.size() > 0 ? &_visibilities[0] : NULL;
        }

        /// Returns a pointer to the first element of the data cube
        /// for the given polarisation.
        complex_t* ptr(const unsigned int p) {
            unsigned int index = p * _nChannels * _nAntennas * _nAntennas;
            return _visibilities.size() > index ? &_visibilities[index] : NULL;
        }

        /// Returns a pointer to the first element of the visibility matrix
        /// for the given channel and polarisation.
        complex_t* ptr(const unsigned int c, const unsigned int p) {
            unsigned int index = (_nAntennas * _nAntennas) * (p * _nChannels + c);
            return _visibilities.size() > index ? &_visibilities[index] : NULL;
        }

        /// Dereferences the visibility data for antennas (i, j),
        /// channel (c) and polarisation (p).
        complex_t& operator() (const unsigned int i, const unsigned int j,
                const unsigned int c, const unsigned int p) {
            int index = i + _nAntennas * (j + _nAntennas * (c + _nChannels * p));
            return _visibilities[index];
        }

        /// Dereferences the visibility data for the given index.
        complex_t& operator() (const unsigned int i) {
            return _visibilities[i];
        }

        /// Returns a reference to the visibility data (use with caution!).
        std::vector<complex_t>& visibilities() {return _visibilities;}

    private: /* Methods */
        /// Initialises the data members.
        void _init();
};

} // namespace pelican

#endif // VISIBILITYDATA_H
