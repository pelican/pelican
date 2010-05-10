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
 * 
 * @details
 * 
 */

class TestDataBlob : public DataBlob
{
    public:
        TestDataBlob(  );
        ~TestDataBlob();
        void setData(const QString& data);
        bool operator==(const TestDataBlob&);
        void deserialise(QIODevice&);
        void serialise(QIODevice&) const;

    private:
        QString _data;
};

} // namespace pelican
#endif // TESTDATABLOB_H 
