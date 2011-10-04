#ifndef DATABLOBBUFFER_H
#define DATABLOBBUFFER_H

#include <QtCore/QVector>


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
 *    At least one DataBlob must be provided otherwise this
 *    is undefined
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
        QVector<DataBlob*> _data;
        unsigned int _index;
        unsigned int _size;
};

} // namespace pelican
#endif // DATABLOBBUFFER_H 
