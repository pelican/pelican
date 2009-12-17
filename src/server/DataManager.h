#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QHash>
//#include "DataBuffer.h"
/**
 * @file DataManager.h
 */

namespace pelican {

class Data;

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

        /// getNextStreamData
        //  return the next unlocked data block from Stream Data
        Data getNextStreamData(const QString& type);

        /// getStreamData
        //  return the requested Service Data
        Data getServiceData(const QString& data);

    private:
        //QHash<DataBuffer> _streams;
        //QHash<DataBuffer> _service;

};

} // namespace pelican
#endif // DATAMANAGER_H 
