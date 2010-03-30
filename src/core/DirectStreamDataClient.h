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
 *    A Data Client that allows you to connect to one or more data streams
 *    directly using specialist chunkers, rather than through the PelicanServer
 *    This is useful when the data rates on the stream can be handled in a single
 *    pipeline.
 * @details
 *    To use:
 *    1) Set up the client in the usual way with adapters etc.
 *    2) add a chunker for each stream
 *    3) setup your configuration file
 *    4) call the start() mehtod to launch all the chunkers in seperate threads
 *    5) now ready to call getData
 *
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
