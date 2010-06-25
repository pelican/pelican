#ifndef TESTDATABLOB_H
#define TESTDATABLOB_H


#include "pelican/data/DataBlob.h"

/**
 * @file TestDataBlob.h
 */

namespace pelican {

/**
 * @class TestDataBlob
 *  
 * @brief
 * Data blob to support unit testing.
 * 
 * @details
 * A minimalist data blob that contains only a string.
 * This data blob is used in unit tests.
 */
class TestDataBlob : public DataBlob
{
    public:
        /// Constructs a TestDataBlob.
        TestDataBlob() : DataBlob("TestDataBlob") {}

        /// Destroys the TestDataBlob.
        ~TestDataBlob() {}

        /// Sets the content of the data blob.
        void setData(const QString& data) {_data = data;}

        /// Tests for equality with another data test blob.
        bool operator==(const TestDataBlob& blob) {return _data == blob._data;}

        /// Deserialises the content from a QIODevice.
        void deserialise(QIODevice&, QSysInfo::Endian);

        /// Serialises the data blob content.
        void serialise(QIODevice&) const;

        /// Returns the number of bytes in the serialised data blob.
        quint64 serialisedBytes() const {return _data.length();}

    private:
        QString _data;
};

PELICAN_DECLARE_DATABLOB(TestDataBlob)

} // namespace pelican

#endif // TESTDATABLOB_H 
