#include "tutorial/SignalData.h"
#include <QtCore/QDataStream>

// Serialises the data blob.
void SignalData::serialise(QIODevice& out) const
{
    QDataStream stream(&out);

    // Write the number of samples in the time series.
    unsigned samples = size();
    stream << samples;

    // Write the data to the output device.
    const float* data = ptr();
    for (unsigned i = 0; i < samples; ++i) stream << data[i];
}

// Deserialises the data blob.
void SignalData::deserialise(QIODevice& in, QSysInfo::Endian)
{
    QDataStream stream(&in);

    // Read the number of samples in the time series.
    unsigned samples = 0;
    stream >> samples;

    // Set the size of the blob and read the data into it.
    resize(samples);
    float* data = ptr();
    for (unsigned i = 0; i < samples; ++i) stream >> data[i];
}
