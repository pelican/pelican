#ifndef CHUNKERMANAGER_H
#define CHUNKERMANAGER_H

/**
 * @file ChunkerManager.h
 */

#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QPair>
#include <QtCore/QVector>
#include <boost/shared_ptr.hpp>

#include "pelican/utility/FactoryConfig.h"
#include "pelican/server/DataReceiver.h"
#include "pelican/utility/Config.h"


namespace pelican {

class AbstractChunker;
class DataManager;

/**
 * @ingroup c_server
 *
 * @class ChunkerManager
 *
 * @brief
 * Manage and operate chunkers.
 *
 * @details
 * Anything coming through a stream data chunker will be associated with the
 * current version of service data service data has no such association.
 */

class ChunkerManager
{
    public:
        /// @deprecated
        /// Creates a chunker manager.
        ChunkerManager(const Config* config, const QString& section = "server");

        /// Creates a chunker manager.
        ChunkerManager(const Config* config,  const Config::TreeAddress& base);

        /// Destroys the chunker manager.
        ~ChunkerManager();

    public:
        /// Initialise the passed data manager to support the contained chunkers.
        bool init(DataManager& dm);

        /// Adds a stream chunker by class name (\p type).
        AbstractChunker* addStreamChunker(QString type, QString name = QString());

        /// Adds a service chunker by class name (\p type).
        AbstractChunker* addServiceChunker(QString type, QString name = QString());

        /// Returns the list of chunkers.
        const QSet<AbstractChunker* >& chunkers() const { return _chunkers; };

        /// return the current device associated with the specified
        //  chunker
        QIODevice* getCurrentDevice( AbstractChunker* chunker ) const;

        /// Returns true if all the data chunkers are operating, otherwise false.
        bool isRunning() const;

    private:
        // Adds an allocated chunker to the manager.
        void _addChunker( AbstractChunker* chunker);

    private:
        FactoryConfig<AbstractChunker> *_factory;
        QMap<QPair<QString,quint16>,AbstractChunker* > _chunkerPortMap;
        QSet<AbstractChunker* > _chunkers;
        QSet<QString> _streamDataTypes;
        QSet<QString> _serviceDataTypes;
        QHash<AbstractChunker*, DataReceiver*> _dataReceivers;
};

} // namespace pelican
#endif // CHUNKERMANAGER_H
