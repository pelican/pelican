#ifndef FREQUENCYLIST_H
#define FREQUENCYLIST_H

#include "data/DataBlob.h"
#include <vector>

/**
 * @file FrequencyList.h
 */

namespace pelican {

/**
 * @class FrequencyList
 *
 * @brief
 * Class to define the channel to frequency mapping.
 * 
 * @details
 * This class defines how channel frequency data is held. It inherits
 * the DataBlob base class.
 *
 * Internally, the data is stored as a contiguous memory
 * block wrapped inside a standard vector.
 */
template<typename T> class FrequencyList : public DataBlob
{
    private: /* Data */
        /// The list of channel-to-frequency mappings.
        std::vector<T> _data;

        /// The number of frequency channels.
        unsigned _nChannels;

    public:
        /// Constructs an empty frequency list.
        FrequencyList() : DataBlob() {}

        /// Frequency list destructor.
        ~FrequencyList() {}

        /// Returns the number of frequency channels.
        unsigned nChannels() const { return _nChannels; }

        /// Returns a pointer to the first element of the memory block.
        T* ptr() { return _data.size() > 0 ? &_data[0] : NULL; }

        /// Resizes the data container.
        /// This public method is used to resize the container using the
        /// specified values.
        ///
        /// @param[in] channels The number of frequency channels.
        void resize(unsigned channels) {
            _nChannels = channels;
            _data.resize(channels);
        }

        /// Returns a reference to the data vector (use with caution!).
        /// This method may be deprecated in due course.
        std::vector<T>& data() { return _data; }

        /// Dereferences the data for channel (\p c).
        T& operator() (const unsigned c) { return _data[c]; }

        /// Dereferences the data for channel (\p c) (const overload).
        const T& operator() (const unsigned c) const { return _data[c]; }

        /// Dereferences the data for the given index \p i.
        T& operator[] (const unsigned c) { return _data[c]; }

        /// Dereferences the data for the given index \p i (const overload).
        const T& operator[] (const unsigned c) const { return _data[c]; }
};

} // namespace pelican

#endif // FREQUENCYLIST_H
