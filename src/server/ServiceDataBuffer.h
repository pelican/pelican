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
 * 
 * @details
 * 
 */

class ServiceDataBuffer : public DataBuffer
{
    public:
        ServiceDataBuffer(  );
        ~ServiceDataBuffer();
        LockedData getData(const QString& version);

    private:
        //QMutex _mutex;
        QHash<QString,Data*> _data;

};

} // namespace pelican
#endif // SERVICEDATABUFFER_H 
