#ifndef ABSTRACTSTREAMDATACLIENT_H
#define ABSTRACTSTREAMDATACLIENT_H

#include "AbstractDataClient.h"

/**
 * @file AbstractStreamDataClient.h
 */

namespace pelican {

/**
 * @class AbstractStreamDataClient
 *  
 * @brief
 *    A Background Thread that reads raw data directly from a socket
 * @details
 *    An interface class
 */

class AbstractStreamDataClient : public AbstractDataClient
{
    public:
        AbstractStreamDataClient( const ConfigNode& config, const DataTypes& types );
        virtual ~AbstractStreamDataClient();
        void setPort(unsigned int port);
        void setIP_Address(const QString& ipaddress);
        QHash<QString, DataBlob*> getData(QHash<QString, DataBlob*>& dataHash);

    protected:
        //virtual void connect(const QString& address, unsigned int port);

    private:
        QString _server;
        unsigned int _port;
};

} // namespace pelican
#endif // ABSTRACTSTREAMDATACLIENT_H 
