#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QHash>
#include "StreamDataBuffer.h"
#include "ServiceDataBuffer.h"
#include "LockedData.h"
#include "data/DataRequirements.h"
#include "utility/Config.h"

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
 * Main class for accessing data in the server.
 *
 * @details
 * Class to control access to the internal server data to ensure
 * locking etc.
 */
class DataManager
{
    private:
        QHash<QString,StreamDataBuffer*> _streams;
        QHash<QString,ServiceDataBuffer*> _service;
        DataRequirements _specs;

    public:
        /// DataManager constructor.
        DataManager(const Config* config);

        /// DataManager destructor.
        virtual ~DataManager();

        /// AssociateServiceData
        void associateServiceData(LockableStreamData* data);

        /// Returns the data types handled by this manager.
        const DataRequirements& dataSpec() const {return _specs;}

        /// Return a list of Stream Data objects corresponding
        //  to a DataRequirement object 
        QList<LockedData> getDataRequirements(const DataRequirements& req);

        /// Return the next unlocked data block from Stream Data.
        /// If the associate data requested is unavailable,
        /// LockedData will be invalid.
        LockedData getNext(const QString& type, const QSet<QString>& associateData);

        /// Return the next unlocked data block from Stream Data.
        LockedData getNext(const QString& type);

        /// Return the requested Service Data.
        LockedData getServiceData(const QString& type, const QString& version);

        /// Set up a stream buffer for the specified type.
        StreamDataBuffer* getStreamBuffer(const QString& type);

        /// Set up a service buffer for the specified type.
        ServiceDataBuffer* getServiceBuffer(const QString& type);

        /// Return a WritableData object that represents
        /// a space in the buffer of a minimum size specified
        /// An invalid Writable object will be returned if
        /// the space is not available.
        WritableData getWritableData(const QString& type, size_t size);

        /// Add a service data type to be managed.
        void setServiceDataBuffer(const QString& name, ServiceDataBuffer* buffer);

        /// Add a stream data type to be managed.
        void setStreamDataBuffer(const QString& name, StreamDataBuffer* buffer);

    public:
        const Config* _config;
        Config::TreeAddress_t _bufferConfigBaseAddress;
};

} // namespace pelican

#endif // DATAMANAGER_H 
