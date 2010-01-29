#ifndef MATRIXDATA_H
#define MATRIXDATA_H

#include "data/DataBlob.h"
#include <vector>

/**
 * @file MatrixData.h
 */

namespace pelican {

/**
 * @class MatrixData
 *
 * @brief
 * Class to define matrix data.
 * 
 * @details
 * This class defines how matrix data is held. It inherits
 * the DataBlob base class, and is used by the VisibilityData,
 * FlagTable and AntennaGains data blobs.
 *
 * Internally, the data is stored as a contiguous memory
 * block wrapped inside a standard vector.
 *
 * A data cube consists of square matrices of data, indexed by
 * antenna (i, j), and stacked in planes of independent frequency
 * channels (c).
 *
 * The data for each polarisation (p) is stored in separate, but contiguous,
 * data cubes.
 *
 * If the
 *
 * The matrix data is stored in column-major order for compatibility
 * with high-performance FORTRAN libraries like LAPACK.
 */
template<typename T> class MatrixData : public DataBlob
{
    private: /* Data */
        /// The data.
        std::vector<T> _data;

        /// Flag to use a 2D antenna matrix
        bool _use2dAntennaMatrix;

        /// The number of antennas.
        unsigned _nAntennas;

        /// The number of frequency channels.
        unsigned _nChannels;

        /// The number of polarisations.
        unsigned _nPolarisations;

    public:
        /// Constructs an empty data cube.
        /// The constructed data cube has zero size.
        MatrixData(bool use2dAntennaMatrix = true) :
            _use2dAntennaMatrix(use2dAntennaMatrix),
            _nAntennas(0), _nChannels(0), _nPolarisations(0),
            DataBlob() {}

        /// Constructs a pre-sized, empty cube.
        /// The cube is pre-sized using the given parameters.
        ///
        /// @param[in] antennas The number of antennas in the visibility matrix.
        /// @param[in] channels The number of frequency channels.
        /// @param[in] polarisations The number of polarisations.
        MatrixData(unsigned antennas, unsigned channels,
                unsigned polarisations, bool use2dAntennaMatrix = true) :
                    _use2dAntennaMatrix(use2dAntennaMatrix),
                    DataBlob() {
            resize(antennas, channels, polarisations);
        }

        /// Matrix data destructor.
        ~MatrixData() {}

        /// Returns the number of antennas.
        unsigned nAntennas() const {return _nAntennas;}

        /// Returns the number of frequency channels.
        unsigned nChannels() const {return _nChannels;}

        /// Returns the number of polarisations.
        unsigned nPolarisations() const {return _nPolarisations;}

        /// Returns a pointer to the first element of the memory block.
        T* ptr() {
            return _data.size() > 0 ? &_data[0] : NULL;
        }

        /// Returns a pointer to the first element of the data cube
        /// for the given polarisation \p p.
        T* ptr(const unsigned p) {
            unsigned index = p * _nChannels * _nAntennas * _nAntennas;
            return _data.size() > index ? &_data[index] : NULL;
        }

        /// Returns a pointer to the first element of the matrix
        /// for the given channel \p c and polarisation \p p.
        T* ptr(const unsigned c, const unsigned p) {
            unsigned index = (_nAntennas * _nAntennas) * (p * _nChannels + c);
            return _data.size() > index ? &_data[index] : NULL;
        }

        /// Resizes the data container.
        /// This public method is used to resize the container using the specified
        /// values.
        ///
        /// @param[in] antennas The number of antennas in the visibility matrix.
        /// @param[in] channels The number of frequency channels.
        /// @param[in] polarisations The number of polarisations.
        void resize(unsigned antennas, unsigned channels,
                unsigned polarisations) {
            _nAntennas = antennas;
            _nChannels = channels;
            _nPolarisations = polarisations;
            _data.resize(antennas * antennas * channels * polarisations);
        }

        /// Returns a reference to the data vector (use with caution!).
        std::vector<T>& data() {return _data;}

        /// Dereferences the data for antennas (i, j),
        /// channel (c) and polarisation (p).
        /// The index \p i is the row number, and the index \p j is the
        /// column number (the matrix is column-major).
        T& operator() (const unsigned i, const unsigned j,
                const unsigned c, const unsigned p) {
            unsigned index = i + _nAntennas * (j + _nAntennas * (c + _nChannels * p));
            return _data[index];
        }

        /// Dereferences the data for antennas (i, j),
        /// channel (c) and polarisation (p) (const overload).
        /// The index \p i is the row number, and the index \p j is the
        /// column number (the matrix is column-major).
        const T& operator() (const unsigned i, const unsigned j,
                const unsigned c, const unsigned p) const {
            unsigned index = i + _nAntennas * (j + _nAntennas * (c + _nChannels * p));
            return _data[index];
        }

        /// Dereferences the data for the given index \p i.
        T& operator() (const unsigned i) {
            return _data[i];
        }

        /// Dereferences the data for the given index \p i (const overload).
        const T& operator() (const unsigned i) const {
            return _data[i];
        }

        /// Dereferences the data for the given index \p i.
        T& operator[] (const unsigned i) {
            return _data[i];
        }

        /// Dereferences the data for the given index \p i (const overload).
        const T& operator[] (const unsigned i) const {
            return _data[i];
        }
};

} // namespace pelican

#endif // MATRIXDATA_H
