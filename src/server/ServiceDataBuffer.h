#ifndef SERVICEDATABUFFER_H
#define SERVICEDATABUFFER_H

#include <QMutex>
#include <QHash>
#include "DataBuffer.h"

/**
 * @file ServiceDataBuffer.h
 */

namespace pelican {

class Data;
class LockedData;

/**
 * @class ServiceDataBuffer
 *  
 * @brief
 *    Maintains service data
 * @details
 *    Access to service data is assumed to be by direct reference
 *    either through an explicit identifier, or through the current()
 *    method.
 *    Multiple threads may access the same Data at the same time for
 *    reading.
 *    
 */

class ServiceDataBuffer : public DataBuffer
{
    Q_OBJECT

    public:
        ServiceDataBuffer(  );
        ~ServiceDataBuffer();
        /// return a specific version of the data
        //  if this data is unavailable then return an invalid
        LockedData getData(const QString& version);

        /// remove the specified version from the buffer
        void retireData(const QString& version);

        /// return the current version
        LockedData getCurrent();

        /// getWritable returns an object for filling
        //  this object is locked and on unlocking
        //  will become the current.
        WritableData getWritable(size_t size);

    public slots:
        void activateData(Data*);

    private:
        QHash<QString,Data*> _data;
        Data* _newData; // temporary store until activated
        QString _current;
        size_t _max;
        size_t _maxChunkSize;
        size_t _space;
        unsigned long _id;
};

} // namespace pelican
#endif // SERVICEDATABUFFER_H 
