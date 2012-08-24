#ifndef DATABLOBVERIFY_H
#define DATABLOBVERIFY_H
#include <QtCore/QBuffer>


/**
 * @file DataBlobVerify.h
 */

namespace pelican {
class DataBlob;

/**
 * @class DataBlobVerify
 *  
 * @brief
 *    A class to aid in ensuring DataBlobs have been
 *    consistently built
 * @details
 * 
 */

class DataBlobVerify
{
    public:
        DataBlobVerify( const DataBlob* );
        ~DataBlobVerify();
        bool verifySerialisedBytes() const;
        bool verifyDeserialise() const;

    protected:
        void _serialise() const;

    private:
        const DataBlob* _blob;
        mutable QBuffer   _buffer;
};

} // namespace pelican
#endif // DATABLOBVERIFY_H 
