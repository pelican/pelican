#ifndef ANTENNAMATRIXDATA_H
#define ANTENNAMATRIXDATA_H

#include "data/DataBlob.h"
#include <vector>

/**
 * @file AntennaMatrixData.h
 */

namespace pelican {

/**
 * @class AntennaMatrixData
 *
 * @brief
 * Class to define antenna matrix data.
 * 
 * @details
 * This class defines how antenna matrix data is held. It inherits
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
 * The matrix data is stored in column-major order for compatibility
 * with high-performance FORTRAN libraries like LAPACK.
 *
 * If the optional argument \p use2dAntennaMatrix is set to false in the
 * constructor, the antenna data will be stored and indexed as a
 * one-dimensional vector rather than a two-dimensional matrix.
 */
template<typename T> class AntennaMatrixData : public DataBlob
{
    protected: /* Data */
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
        ///
        /// @param[in] use2dAntennaMatrix If set, use a 2D antenna matrix (default true).
        AntennaMatrixData(bool use2dAntennaMatrix = true) : DataBlob(),
            _use2dAntennaMatrix(use2dAntennaMatrix),
            _nAntennas(0), _nChannels(0), _nPolarisations(0) {}

        /// Constructs a pre-sized, empty cube.
        /// The cube is pre-sized using the given parameters.
        ///
        /// @param[in] antennas The number of antennas in the visibility matrix.
        /// @param[in] channels The number of frequency channels.
        /// @param[in] polarisations The number of polarisations.
        /// @param[in] use2dAntennaMatrix If set, use a 2D antenna matrix (default true).
        AntennaMatrixData(unsigned antennas, unsigned channels,
                unsigned polarisations, bool use2dAntennaMatrix = true) : DataBlob(),
                _use2dAntennaMatrix(use2dAntennaMatrix) {
            resize(antennas, channels, polarisations);
        }

        /// Matrix data destructor.
        ~AntennaMatrixData() {}

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
            unsigned index = p * _nChannels * _nAntennas;
            if (_use2dAntennaMatrix) index *= _nAntennas;
            return _data.size() > index ? &_data[index] : NULL;
        }

        /// Returns a pointer to the first element of the matrix
        /// for the given channel \p c and polarisation \p p.
        T* ptr(const unsigned c, const unsigned p) {
            unsigned a = (_use2dAntennaMatrix) ? _nAntennas * _nAntennas : _nAntennas;
            unsigned index = a * (p * _nChannels + c);
            return _data.size() > index ? &_data[index] : NULL;
        }

        /// Resizes the data container.
        /// This public method is used to resize the container using the
        /// specified values. The container is resized appropriately depending
        /// on whether the antenna data is one- or two-dimensional.
        ///
        /// @param[in] antennas The number of antennas in the visibility matrix.
        /// @param[in] channels The number of frequency channels.
        /// @param[in] polarisations The number of polarisations.
        void resize(unsigned antennas, unsigned channels,
                unsigned polarisations) {
            _nAntennas = antennas;
            _nChannels = channels;
            _nPolarisations = polarisations;
            unsigned a = (_use2dAntennaMatrix) ? antennas * antennas : antennas;
            _data.resize(a * channels * polarisations);
        }

        /// Swaps the two rows and columns in the antenna matrix.
        /// This function swaps the row and column of the specified antenna
        /// indices. It can be used to compact 'good' visibility data when
        /// flagging bad antennas. Note that the index is the current row and
        /// column of the antenna, not the original one (which may have been
        /// changed by prior calls to this function).
        ///
        /// <b>Important:</b> This function can only be used for 2D antenna data.
        ///
        /// @param[in] index1 The row and column index to swap.
        /// @param[in] index2 The row and column index to swap.
        /// @param[in] channel The channel index.
        /// @param[in] polarisation The polarisation index.
        void swapAntennaData2d(unsigned index1, unsigned index2,
                unsigned channel, unsigned polarisation)
        {
            T *mptr = ptr(channel, polarisation); // Matrix pointer.

            /* Swap the columns */
            T *ptrc1 = mptr + index1 * _nAntennas;
            T *ptrc2 = mptr + index2 * _nAntennas;
            for (unsigned row = 0; row < _nAntennas; ++row) {
                const T tmp = ptrc1[row];
                ptrc1[row]  = ptrc2[row];
                ptrc2[row]  = tmp;
            }

            /* Swap the rows */
            for (unsigned col = 0; col < _nAntennas; ++col) {
                const unsigned offset = col * _nAntennas;
                const unsigned pos1 = index1 + offset;
                const unsigned pos2 = index2 + offset;

                const T tmp = mptr[pos1];
                mptr[pos1]  = mptr[pos2];
                mptr[pos2]  = tmp;
            }
        }

        /// Returns a reference to the data vector (use with caution!).
        /// This method may be deprecated in due course.
        std::vector<T>& data() {return _data;}

        /// Dereferences the data for antennas (\p ai, \p aj),
        /// channel (\p c) and polarisation (\p p).
        /// The index \p ai is the row number, and the index \p aj is the
        /// column number (the matrix is column-major).
        ///
        /// This is used when the antenna matrix is two-dimensional.
        T& operator() (const unsigned ai, const unsigned aj,
                const unsigned c, const unsigned p) {
            unsigned index = ai + _nAntennas * (aj + _nAntennas * (c + _nChannels * p));
            return _data[index];
        }

        /// Dereferences the data for antennas (\p ai, \p aj),
        /// channel (\p c) and polarisation (\p p) (const overload).
        /// The index \p ai is the row number, and the index \p aj is the
        /// column number (the matrix is column-major).
        ///
        /// This is used when the antenna matrix is two-dimensional.
        const T& operator() (const unsigned ai, const unsigned aj,
                const unsigned c, const unsigned p) const {
            unsigned index = ai + _nAntennas * (aj + _nAntennas * (c + _nChannels * p));
            return _data[index];
        }

        /// Dereferences the data for antenna (\p a),
        /// channel (\p c) and polarisation (\p p).
        ///
        /// This is used when the antenna matrix is NOT two-dimensional.
        T& operator() (const unsigned a, const unsigned c, const unsigned p) {
            unsigned index = a + _nAntennas * (c + _nChannels * p);
            return _data[index];
        }

        /// Dereferences the data for antenna (\p a),
        /// channel (\p c) and polarisation (\p p) (const overload).
        ///
        /// This is used when the antenna matrix is NOT two-dimensional.
        const T& operator() (const unsigned a, const unsigned c,
                const unsigned p) const {
            unsigned index = a + _nAntennas * (c + _nChannels * p);
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

#endif // ANTENNAMATRIXDATA_H
