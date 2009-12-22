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
        DataManager(  );
        ~DataManager();

        /// returns the data types handled by this manager
        const DataRequirements& dataSpec() const;

        /// getNext
        //  return the next unlocked data block from Stream Data
        LockedData getNext(const QString& type);

        /// getServiceData
        //  return the requested Service Data
        LockedData getServiceData(const QString& type, const QString& version);

        /// serviceDataBuffer
        //  add a service data type to be managed
        void serviceDataBuffer(const QString& name, const ServiceDataBuffer& buffer);

        /// streamDataBuffer
        //  add a stream data type to be managed
        void streamDataBuffer(const QString& name, const StreamDataBuffer& buffer);

    private:
        QHash<QString,StreamDataBuffer> _streams;
        QHash<QString,ServiceDataBuffer> _service;
        DataRequirements _specs;

};

} // namespace pelican
#endif // DATAMANAGER_H 
