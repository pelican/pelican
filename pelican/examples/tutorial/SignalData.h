#ifndef SIGNALDATA_H
#define SIGNALDATA_H

#include "data/DataBlob.h"
#include <vector>

using namespace pelican;

/*
 * This data blob holds an array of floating-point time-stream data.
 */
class SignalData : public DataBlob
{
    public:
        // Constructs a signal data blob.
        SignalData() : DataBlob("SignalData") {}

        // Returns a const pointer to the start of the data.
        const float* ptr() const {return (_data.size() > 0 ? &_data[0] : NULL);}

        // Returns a pointer to the start of the data.
        float* ptr() { return (_data.size() > 0 ? &_data[0] : NULL); }

        // Resizes the data store provided by the data blob.
        void resize(unsigned length) { _data.resize(length); }

        // Returns the size of the data.
        unsigned size() const { return _data.size(); }

        // Serialises the data blob.
        void serialise(QIODevice& out) const;

        // Deserialises the data blob.
        void deserialise(QIODevice& in, QSysInfo::Endian);

    private:
        std::vector<float> _data; // The actual data array.
};

PELICAN_DECLARE_DATABLOB(SignalData)

#endif // SIGNALDATA_H
