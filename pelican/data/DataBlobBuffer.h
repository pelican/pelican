#ifndef DATABLOBBUFFER_H
#define DATABLOBBUFFER_H

#include <QtCore/QList>


/**
 * @file DataBlobBuffer.h
 */

namespace pelican {

/**
 * @class DataBlobBuffer
 *  
 * @brief
 *    A circular buffer of DataBlobs
 * @details
 *   DataBlobs are either active or inactive
 *   inactive blobs can be removed
 */

class ConfigNode;
class DataBlob;

class DataBlobBuffer
{
    public:
        DataBlobBuffer();
        ~DataBlobBuffer();
        void addDataBlob(DataBlob*);
        DataBlob* next();

    private:
        QList<DataBlob*> _data;
        unsigned int _index;
};

} // namespace pelican
#endif // DATABLOBBUFFER_H 
