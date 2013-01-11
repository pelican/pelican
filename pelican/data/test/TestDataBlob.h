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

#ifndef TESTDATABLOB_H
#define TESTDATABLOB_H

/**
 * @file TestDataBlob.h
 */

#include "data/DataBlob.h"
#include <QtCore/QByteArray>

namespace pelican {
namespace test {

/**
 * @ingroup t_data
 *
 * @class TestDataBlob
 *
 * @brief
 * Data blob to support unit testing.
 *
 * @details
 * A minimal data blob that contains only a byte array.
 * This data blob is used in unit tests.
 */

class TestDataBlob : public DataBlob
{
    public:
        /// Constructs a TestDataBlob.
        TestDataBlob( const QString type = "TestDataBlob" ) : DataBlob(type) {}

        /// Destroys the TestDataBlob.
        ~TestDataBlob() {}

        size_t size() const { return _data.size(); }

    public:
        /// Sets the content of the data blob.
        void setData(const QByteArray& data) { _data = data; }

        /// Returns a reference to the byte array that this data blob wraps.
        QByteArray& data() { return _data; }
        const QByteArray& data() const { return _data; }

        /// De-serialises the content from a QIODevice.
        virtual void deserialise(QIODevice&, QSysInfo::Endian endianness);

        /// Resizes the byte array in the data blob.
        void resize(int size) { _data.resize(size); }

        /// Serialises the data blob content.
        void serialise(QIODevice&) const;

        /// Returns the number of bytes in the serialised data blob.
        quint64 serialisedBytes() const { return _data.size() + sizeof(int); }

        /// Tests for equality with another data test blob.
        bool operator==(const TestDataBlob& blob) { return _data == blob._data; }

    private:
        QByteArray _data;
};

PELICAN_DECLARE_DATABLOB(TestDataBlob)

} // namespace test
} // namespace pelican
#endif // TESTDATABLOB_H
