#ifndef DIRECTSTREAMDATACLIENT_H
#define DIRECTSTREAMDATACLIENT_H

#include <QThread>
#include <QMutex>
#include "pelican/core/AbstractDataClient.h"
#include "pelican/server/DataManager.h"

/**
 * @file DirectStreamDataClient.h
 */

namespace pelican {

class ChunkerManager;
class Config;

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
 *    4) call the start() method to launch all the chunkers in separate threads
 *    5) now ready to call getData
 *
 */

class DirectStreamDataClient : public AbstractDataClient, public QThread
{
    public:
        DirectStreamDataClient( const ConfigNode& configNode);
        virtual ~DirectStreamDataClient();
        void setPort(unsigned int port);
        void setIP_Address(const QString& ipaddress);
        QHash<QString, DataBlob*> getData(QHash<QString, DataBlob*>& dataHash);

//        /// Sets the chunker for the given data type (DEPRECATED).
//        void setChunker(const QString& dataType, const QString& chunkerType,
//                const QString& chunkerName = QString());

        /// Sets the service chunker for the given data type.
        void addServiceChunker(const QString& chunkerType,
                const QString& chunkerName = QString());

        /// Sets the stream chunker for the given data type.
        void addStreamChunker(const QString& chunkerType,
                const QString& chunkerName = QString());

        void setManagers(const Config* config);

    protected:
        /// Runs the thread owned by this data client.
        void run();

    protected:
        bool _started;
        //virtual void connect(const QString& address, unsigned int port);

    private:
        QMutex _mutex;
        const Config* _config;
        ChunkerManager* _chunkerManager;
        DataManager* _dataManager;
        QString _server;
        unsigned int _port;
};

} // namespace pelican

#endif // DIRECTSTREAMDATACLIENT_H
