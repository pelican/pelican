#ifndef TESTDATABLOB_H
#define TESTDATABLOB_H

/**
 * @file TestDataBlob.h
 */

#include "pelican/data/DataBlob.h"
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
