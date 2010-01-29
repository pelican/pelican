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
        unsigned _nAntennas;

        /// The number of frequency channels.
        unsigned _nChannels;

        /// The number of polarisations.
        unsigned _nPolarisations;

    public:
        /// Constructs an empty visibility cube.
        VisibilityData();

        /// Constructs a pre-sized visibility cube.
        VisibilityData(unsigned antennas, unsigned channels,
                unsigned polarisations);

        /// Visibility data destructor.
        ~VisibilityData();

        /// Returns the number of antennas.
        unsigned nAntennas() const {return _nAntennas;}

        /// Returns the number of frequency channels.
        unsigned nChannels() const {return _nChannels;}

        /// Returns the number of polarisations.
        unsigned nPolarisations() const {return _nPolarisations;}

        /// Returns a pointer to the first element of the memory block.
        complex_t* ptr() {
            return _visibilities.size() > 0 ? &_visibilities[0] : NULL;
        }

        /// Returns a pointer to the first element of the data cube
        /// for the given polarisation.
        complex_t* ptr(const unsigned p) {
            unsigned index = p * _nChannels * _nAntennas * _nAntennas;
            return _visibilities.size() > index ? &_visibilities[index] : NULL;
        }

        /// Returns a pointer to the first element of the visibility matrix
        /// for the given channel and polarisation.
        complex_t* ptr(const unsigned c, const unsigned p) {
            unsigned index = (_nAntennas * _nAntennas) * (p * _nChannels + c);
            return _visibilities.size() > index ? &_visibilities[index] : NULL;
        }

        /// Resizes the visibility data container.
        void resize(unsigned antennas, unsigned channels,
                unsigned polarisations);

        /// Returns a reference to the visibility data vector (use with caution!).
        std::vector<complex_t>& visibilities() {return _visibilities;}

        /// Dereferences the visibility data for antennas (i, j),
        /// channel (c) and polarisation (p).
        /// The index \p i is the row number, and the index \p j is the
        /// column number (the matrix is column-major).
        complex_t& operator() (const unsigned i, const unsigned j,
                const unsigned c, const unsigned p) {
            unsigned index = i + _nAntennas * (j + _nAntennas * (c + _nChannels * p));
            return _visibilities[index];
        }

        /// Dereferences the visibility data for antennas (i, j),
        /// channel (c) and polarisation (p) (const overload).
        /// The index \p i is the row number, and the index \p j is the
        /// column number (the matrix is column-major).
        const complex_t& operator() (const unsigned i, const unsigned j,
                const unsigned c, const unsigned p) const {
            unsigned index = i + _nAntennas * (j + _nAntennas * (c + _nChannels * p));
            return _visibilities[index];
        }

        /// Dereferences the visibility data for the given index.
        complex_t& operator() (const unsigned i) {
            return _visibilities[i];
        }

        /// Dereferences the visibility data for the given index (const overload).
        const complex_t& operator() (const unsigned i) const {
            return _visibilities[i];
        }

        /// Dereferences the visibility data for the given index.
        complex_t& operator[] (const unsigned i) {
            return _visibilities[i];
        }

        /// Dereferences the visibility data for the given index (const overload).
        const complex_t& operator[] (const unsigned i) const {
            return _visibilities[i];
        }
};

} // namespace pelican

#endif // VISIBILITYDATA_H
