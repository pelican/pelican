#ifndef TIMESTREAMDATA_H_
#define TIMESTREAMDATA_H_

/**
 * @file TimeStreamData.h
 */

#include "pelican/data/DataBlob.h"
#include <vector>

namespace pelican {

/**
 * @class TimeStreamData
 *
 * @brief
 * Container class to hold a buffer of time stream data.
 *
 * @details
 * Used for time domain processing application such as channeliser modules.
 */

template <class T>
class TimeStreamData : public DataBlob
{
    public:
        /// Constructs an empty time stream data blob.
        TimeStreamData() : DataBlob(), _startTime(0.0), _sampleDelta(0.0) {}

        /// Constructs and assigns memory for a time stream buffer data blob.
        TimeStreamData(const unsigned nTimeSamples) : DataBlob() {
            resize(nTimeSamples);
        }

    public:
        /// Clears the time stream data.
        void clear() {
            _data.clear();
            _startTime = 0.0;
            _sampleDelta = 0.0;
        }

        /// Assign memory for the time stream data blob.
        void resize(const unsigned nTimeSamples) {
            _data.resize(nTimeSamples);
        }

    public: // accessor methods
        /// Returns a pointer to the time stream data.
        T* data() { return _data.size() > 0 ? _data[0] : NULL; }

        /// Returns a pointer to the time stream data (const overload).
        const T* data() const  { return _data.size() > 0 ? _data[0] : NULL; }

        /// Returns the start time of the data.
        double startTime() const { return _startTime; }

        /// Sets the start time of the data.
        void setStartTime(const double value) { _startTime = value; }

        /// Returns the sample delta.
        double sampleDelta() const { return _sampleDelta; }

        /// Sets the time interval between samples.
        void setSampleDelta(const double value) { _sampleDelta = value; }

    private:
        std::vector<T> _data;
        double _startTime;
        double _sampleDelta;
};


}// namespace pelican

#endif // TIMESTREAMDATA_H_
