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
 * The memory layout is as shown in the figure below, using visibility data
 * or complex antenna gains as an example:
 *
 * \image html doc/VisibilityData.png
 * \image latex doc/VisibilityData.eps "AntennaMatrixData memory layout." width=10cm
 *
 * The matrix data is stored in column-major order for compatibility
 * with high-performance FORTRAN libraries like LAPACK.
 *
 * The antenna index data is stored separately, and is used to convert row and
 * column indices to a physical antenna index. The list of antenna index data is
 * stored for each channel and polarisation.
 */
template<typename T> class AntennaMatrixData : public DataBlob
{
    protected: /* Data */
        /// The data.
        std::vector<T> _data;

        /// The antenna index data.
        std::vector<unsigned> _antIndex;

        /// The number of antennas.
        unsigned _nAntennas;

        /// The number of frequency channels.
        unsigned _nChannels;

        /// The number of polarisations.
        unsigned _nPolarisations;

    public:
        /// Constructs an empty data cube.
        /// The constructed data cube has zero size.
        AntennaMatrixData() : DataBlob(),
            _nAntennas(0), _nChannels(0), _nPolarisations(0) {}

        /// Constructs a pre-sized, empty cube.
        /// The cube is pre-sized using the given parameters, and the antenna
        /// indices are initialised to increase along the rows and columns,
        /// for each polarisation and frequency.
        ///
        /// @param[in] antennas The number of antennas in the visibility matrix.
        /// @param[in] channels The number of frequency channels.
        /// @param[in] polarisations The number of polarisations.
        AntennaMatrixData(const unsigned antennas, const unsigned channels,
                const unsigned polarisations) : DataBlob() {
            resize(antennas, channels, polarisations);
            initAntennaIndex();
        }

        /// Matrix data destructor.
        ~AntennaMatrixData() {}

        /// Returns the antenna index associated with a row and column.
        /// This method returns the real antenna index in row and column \p i
        /// for the antenna matrix at channel \p c and polarisation \p p.
        ///
        /// It may not be the same as the row and column index, if the
        /// visibility data has been re-ordered by the flagging module.
        unsigned antennaIndex(const unsigned i, const unsigned c,
                const unsigned p) const {
            return _antIndex[(p * _nChannels + c) * _nAntennas + i];
        }

        /// Returns the antenna index data pointer.
        /// This method returns a pointer to the first element of the antenna
        /// index data.
        ///
        /// If there is no antenna index data, a null pointer is returned.
        unsigned* antennaIndexPtr() const {
            return (_antIndex.size() > 0) ? &_antIndex[0] : NULL;
        }

        /// Returns the antenna index data pointer.
        /// This method returns a pointer to the element of the antenna
        /// index data at channel \p c and polarisation \p.
        ///
        /// If there is no antenna index data, a null pointer is returned.
        unsigned* antennaIndexPtr(const unsigned c, const unsigned p) const {
            unsigned i = (p * _nChannels + c) * _nAntennas;
            return (_antIndex.size() > i) ? &_antIndex[i] : NULL;
        }

        /// Initialises the antenna index data.
        /// This method initialises the antenna indices by making them the
        /// same as the matrix row and column numbers.
        void initAntennaIndex() {
            _antIndex.resize(_nAntennas * _nChannels * _nPolarisations);
            for (unsigned p = 0, i = 0; p < _nPolarisations; p++) {
                for (unsigned c = 0; c < _nChannels; c++) {
                    for (unsigned a = 0; a < _nAntennas; a++, i++) {
                        _antIndex[i] = a;
                    }
                }
            }
        }

        /// Returns the number of entries in the antenna matrix data.
        unsigned nEntries() const { return _data.size(); }

        /// Returns the number of antennas.
        unsigned nAntennas() const { return _nAntennas; }

        /// Returns the number of frequency channels.
        unsigned nChannels() const { return _nChannels; }

        /// Returns the number of polarisations.
        unsigned nPolarisations() const { return _nPolarisations; }

        /// Returns a pointer to the start of the memory block.
        /// This method returns a pointer to the first element of the
        /// memory block used for all matrix data.
        ///
        /// If the data cube has no size, a null pointer is returned.
        T* ptr() { return _data.size() > 0 ? &_data[0] : NULL; }

        /// Returns a pointer to the start of the memory block.
        /// This method returns a pointer to the first element of the
        /// memory block used for all matrix data (const overload).
        ///
        /// If the data cube has no size, a null pointer is returned.
        const T* ptr() const { return _data.size() > 0 ? &_data[0] : NULL; }

        /// Returns a pointer to the start of polarisation \p p.
        /// This method returns a pointer to the first element of the data
        /// cube for the given polarisation \p p.
        ///
        /// If the data cube has no size, a null pointer is returned.
        T* ptr(const unsigned p) {
            unsigned index = p * _nChannels * _nAntennas * _nAntennas;
            return _data.size() > index ? &_data[index] : NULL;
        }

        /// Returns a pointer to the start of polarisation \p p.
        /// This method returns a pointer to the first element of the data
        /// cube for the given polarisation \p p (const overload).
        ///
        /// If the data cube has no size, a null pointer is returned.
        const T* ptr(const unsigned p) const {
            unsigned index = p * _nChannels * _nAntennas * _nAntennas;
            return _data.size() > index ? &_data[index] : NULL;
        }

        /// Returns a pointer to the start of the antenna matrix.
        /// This method returns a pointer to the first element of the antenna
        /// matrix for the given channel \p c and polarisation \p p.
        ///
        /// If the data cube has no size, a null pointer is returned.
        T* ptr(const unsigned c, const unsigned p) {
            unsigned index = _nAntennas * _nAntennas * (p * _nChannels + c);
            return _data.size() > index ? &_data[index] : NULL;
        }

        /// Returns a pointer to the start of the antenna matrix.
        /// This method returns a pointer to the first element of the antenna
        /// matrix for the given channel \p c and polarisation \p p
        /// (const overload).
        ///
        /// If the data cube has no size, a null pointer is returned.
        const T* ptr(const unsigned c, const unsigned p) const {
            unsigned index = _nAntennas * _nAntennas * (p * _nChannels + c);
            return _data.size() > index ? &_data[index] : NULL;
        }

        /// Resizes the data container.
        /// This method is used to resize the container using the
        /// specified values.
        ///
        /// @param[in] antennas The number of antennas in the visibility matrix.
        /// @param[in] channels The number of frequency channels.
        /// @param[in] polarisations The number of polarisations.
        void resize(const unsigned antennas, const unsigned channels,
                const unsigned polarisations) {
            _nAntennas = antennas;
            _nChannels = channels;
            _nPolarisations = polarisations;
            _data.resize(antennas * antennas * channels * polarisations);
        }

        /// Swaps the two rows and columns in the antenna matrix.
        /// This function swaps the row and column of the specified antenna
        /// indices. It can be used to compact 'good' visibility data after
        /// flagging bad antennas. Note that the index is the current row and
        /// column of the antenna, not the original one (which may have been
        /// changed by prior calls to this function).
        ///
        /// @param[in] index1 The row and column index to swap.
        /// @param[in] index2 The row and column index to swap.
        /// @param[in] channel The channel index.
        /// @param[in] polarisation The polarisation index.
        void swapAntennaData(const unsigned index1, const unsigned index2,
                const unsigned channel, const unsigned polarisation)
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

            /* Swap the antenna indices */
            const unsigned as = _nAntennas * (channel + polarisation * _nChannels);
            const unsigned a1 = as + index1;
            const unsigned a2 = as + index2;
            const unsigned tmp = _antIndex[a1];
            _antIndex[a1] = _antIndex[a2];
            _antIndex[a2] = tmp;
        }

        /// Returns a reference to the data vector (use with caution!).
        /// This method may be deprecated in due course.
        std::vector<T>& data() { return _data; }

        /// Dereferences the data for antennas (\p ai, \p aj),
        /// channel (\p c) and polarisation (\p p).
        /// The index \p ai is the row number, and the index \p aj is the
        /// column number (the matrix is column-major).
        T& operator() (const unsigned ai, const unsigned aj,
                const unsigned c, const unsigned p) {
            unsigned index = ai + _nAntennas * (aj + _nAntennas * (c + _nChannels * p));
            return _data[index];
        }

        /// Dereferences the data for antennas (\p ai, \p aj),
        /// channel (\p c) and polarisation (\p p) (const overload).
        /// The index \p ai is the row number, and the index \p aj is the
        /// column number (the matrix is column-major).
        const T& operator() (const unsigned ai, const unsigned aj,
                const unsigned c, const unsigned p) const {
            unsigned index = ai + _nAntennas * (aj + _nAntennas * (c + _nChannels * p));
            return _data[index];
        }

        /// Dereferences the data for the given index \p i.
        T& operator() (const unsigned i) { return _data[i]; }

        /// Dereferences the data for the given index \p i (const overload).
        const T& operator() (const unsigned i) const { return _data[i]; }

        /// Dereferences the data for the given index \p i.
        T& operator[] (const unsigned i) { return _data[i]; }

        /// Dereferences the data for the given index \p i (const overload).
        const T& operator[] (const unsigned i) const { return _data[i]; }
};

} // namespace pelican

#endif // ANTENNAMATRIXDATA_H
