#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QHash>
#include "StreamDataBuffer.h"
#include "ServiceDataBuffer.h"
#include "LockedData.h"
#include "data/DataRequirements.h"
/**
 * @file DataManager.h
 */

namespace pelican {

class Data;
class StreamData;

/**
 * @class DataManager
 *  
 * @brief
 *   Main class for accessing data in the server
 * @details
 *   Class to control access to the internal server data to ensure 
 *   locking etc.
 */

class DataManager
{
    public:
        DataManager();
        ~DataManager();

        /// returns the data types handled by this manager
        const DataRequirements& dataSpec() const;

        /// getNext
        //  return the next unlocked data block from Stream Data
        LockedData getNext(const QString& type);

        /// getNext
        //  return the next unlocked data block from Stream Data, 
        //  If the associate data requested is unavailable then
        //  LockedData will be invalid
        LockedData getNext(const QString& type, const QSet<QString>& associateData );

        /// getServiceData
        //  return the requested Service Data
        LockedData getServiceData(const QString& type, const QString& version);

        /// getWritableData
        //  return a Wriatble data object that represents
        //  a space in the buffer of a minimum size specified  
        //  An invalid Writable object will be returned if
        //  the space is not available
        WritableData getWritableData(const QString& type, size_t size);

        /// associateServiceData
        void associateServiceData(StreamData* data);

        /// serviceDataBuffer
        //  add a service data type to be managed
        void serviceDataBuffer(const QString& name, ServiceDataBuffer* buffer);

        /// streamDataBuffer
        //  add a stream data type to be managed
        void streamDataBuffer(const QString& name, StreamDataBuffer* buffer);

    private:
        QHash<QString,StreamDataBuffer*> _streams;
        QHash<QString,ServiceDataBuffer*> _service;
        DataRequirements _specs;

};

} // namespace pelican
#endif // DATAMANAGER_H 
