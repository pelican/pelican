/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "StreamData.hpp"

#include <QtCore/QDataStream>

StreamData::StreamData() : DataBlob("SignalData")
{
}

const float* StreamData::ptr() const
{
    return data_.size() > 0 ? data_.constData() : 0;
}

float* StreamData::ptr()
{
    return data_.size() > 0 ? data_.data() : 0;
}

void StreamData::resize(int length)
{
    data_.resize(length);
}

int StreamData::size() const
{
    return data_.size();
}

void StreamData::serialise(QIODevice& out) const
{
    QDataStream stream(&out);

    // Write the number of samples in the time series.
    quint32 numSamples = size();
    stream << numSamples;

    // Write the data to the output device.
    const float* data = ptr();
    for (quint32 i = 0; i < numSamples; ++i)
    {
        stream << data[i];
    }
}

void StreamData::deserialise(QIODevice& in, QSysInfo::Endian)
{
    QDataStream stream(&in);

    // Read the number of samples in the time series.
    quint32 numSamples = 0;
    stream >> numSamples;

    // Read data into the blob
    resize(numSamples);
    float* data = ptr();
    for (quint32 i = 0; i < numSamples; ++i)
    {
        stream >> data[i];
    }
}
