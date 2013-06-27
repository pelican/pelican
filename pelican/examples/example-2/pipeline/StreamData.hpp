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


#ifndef STREAMDATA_HPP_
#define STREAMDATA_HPP_

#include "pelican/data/DataBlob.h"
#include <QtCore/QVector>

class StreamData : public pelican::DataBlob
{
public:
    StreamData() : DataBlob("StreamData") {}
    quint32 packetSize() const { return packetSize_; }
    void setPacketSize(quint32 value) { packetSize_ = value; }
    quint32 packetId() const { return packetId_; }
    void setPacketId(quint32 value) { packetId_ = value; }
    quint32 totalPackets() const { return numTotalPackets_; }
    void setTotalPackets(quint32 value) { numTotalPackets_ = value; }
    quint32 timeStamp() const { return timeStamp_; }
    void setTimeStamp(quint32 value) { timeStamp_ = value; }
    void resize(int n) { values_.resize(n); }
    size_t size() const { return values_.size(); }
    const quint32* ptr() const { return values_.data(); }
    quint32* ptr() { return values_.data(); }

private:
    quint32 packetSize_;
    quint32 packetId_;
    quint32 numTotalPackets_;
    quint32 timeStamp_;
    QVector<quint32> values_;
};

PELICAN_DECLARE_DATABLOB(StreamData)


#endif /* STREAMDATA_HPP_ */
