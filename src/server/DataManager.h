#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QHash>
#include "StreamDataBuffer.h"
#include "ServiceDataBuffer.h"
#include "LockedData.h"
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

        /// streams
        //  return a list of streams that the dataManager can
        //  provide. This does NOT reflect if this data is
        //  actually available within thoses streams.
        QList<QString> streams() const;

        /// serviceData
        //  return a list of service data that the dataManager can
        //  provide. This does NOT reflect if this data is
        //  actually available.
        QList<QString> serviceData() const;

        /// getNext
        //  return the next unlocked data block from Stream Data
        LockedData getNext(const QString& type);

        /// getServiceData
        //  return the requested Service Data
        LockedData getServiceData(const QString& type, const QString& version);

    private:
        QHash<QString,StreamDataBuffer> _streams;
        QHash<QString,ServiceDataBuffer> _service;

};

} // namespace pelican
#endif // DATAMANAGER_H 
