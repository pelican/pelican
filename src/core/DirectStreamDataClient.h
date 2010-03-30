#ifndef DIRECTSTREAMDATACLIENT_H
#define DIRECTSTREAMDATACLIENT_H

#include "AbstractDataClient.h"
#include "server/ChunkerManager.h"
#include "server/DataManager.h"

/**
 * @file DirectStreamDataClient.h
 */

namespace pelican {

/**
 * @class DirectStreamDataClient
 *  
 * @brief
 *    A Background Thread that reads raw data directly from a socket
 * @details
 *    An interface class
 */

class DirectStreamDataClient : public AbstractDataClient
{
    public:
        DirectStreamDataClient( const ConfigNode& config, const DataTypes& types );
        virtual ~DirectStreamDataClient();
        void setPort(unsigned int port);
        void setIP_Address(const QString& ipaddress);
        QHash<QString, DataBlob*> getData(QHash<QString, DataBlob*>& dataHash);
        void setChunker(AbstractChunker* chunker, const QString& name);
        void start();

    protected:
        bool _started;
        //virtual void connect(const QString& address, unsigned int port);

    private:
        ChunkerManager _chunkerManager;
        DataManager _dataManager;
        QString _server;
        unsigned int _port;
};

} // namespace pelican
#endif // DIRECTSTREAMDATACLIENT_H 
