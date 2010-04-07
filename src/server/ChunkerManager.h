#ifndef CHUNKERMANAGER_H
#define CHUNKERMANAGER_H

#include <QSet>
#include <QMap>
#include <QString>
#include <QPair>
#include <QVector>
#include <boost/shared_ptr.hpp>
#include "server/DataReceiver.h"

/**
 * @file ChunkerManager.h
 */

namespace pelican {
    class AbstractChunker;
    class DataManager;

/**
 * @class ChunkerManager
 *  
 * @brief
 *    Manage and operatoe chunkers
 * @details
 *    Anything comming througbh a Stream Data chunker
 *    will be associated with the current version of service data
 *    service data has no such association
 */

class ChunkerManager
{
    public:
        ChunkerManager( );
        ~ChunkerManager();

        /// initialise the passed datamanager to support the contained chunkers
        void init(DataManager& dm);

        /// add a Chunker as a Stream Chunker
        //  This will take ownership of the chunker
        //  (see above for more info)
        void addStreamChunker( AbstractChunker* );
        /// add a Chunker as a Service Chunker
        //  This will take ownership of the chunker
        void addServiceChunker( AbstractChunker* );

    private:
        void _addChunker( AbstractChunker* chunker);

    private:
        QMap<QPair<QString,quint16>,AbstractChunker* > _chunkerPortMap;
        QSet<QString> _streamDataTypes;
        QSet<QString> _serviceDataTypes;
        QVector<boost::shared_ptr<DataReceiver> > _dataReceivers;
};

} // namespace pelican
#endif // CHUNKERMANAGER_H 
