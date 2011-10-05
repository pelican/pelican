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

        /// add a new DataBlob for use in the buffer
        void addDataBlob(DataBlob*);

        /// get the next DataBlob from the buffer
        DataBlob* next();

        /// reduce the size of the buffer to the specified size
        void shrink(int newSize);

        /// return the size (number of DataBlobs) held in the Buffer
        unsigned int size() { return _size; }

    private:
        QList<DataBlob*> _data;
        unsigned int _index;
        unsigned int _size;
};

} // namespace pelican
#endif // DATABLOBBUFFER_H 
