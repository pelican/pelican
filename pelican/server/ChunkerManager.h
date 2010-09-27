#ifndef CHUNKERMANAGER_H
#define CHUNKERMANAGER_H

/**
 * @file ChunkerManager.h
 */


#include "pelican/utility/FactoryConfig.h"
#include "pelican/server/DataReceiver.h"
#include "pelican/utility/Config.h"

#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QPair>
#include <QtCore/QVector>

#include <boost/shared_ptr.hpp>

namespace pelican {

class AbstractChunker;
class DataManager;

/**
 * @class ChunkerManager
 *
 * @brief
 * Manage and operate chunkers.
 *
 * @details
 * Anything coming through a stream data chunker
 * will be associated with the current version of service data
 * service data has no such association.
 */
class ChunkerManager
{
    public:
        /// Creates a chunker manager. DO NOT USE.
        /// @deprecated
        ChunkerManager(const Config* config,
                const QString& section = QString("server"));

        /// Creates a chunker manager.
        ChunkerManager(const Config* config,  const Config::TreeAddress& base);

        /// Destroys the chunker manager.
        ~ChunkerManager();

        /// Initialise the passed data manager to support the contained chunkers.
        bool init(DataManager& dm);

        /// Adds a stream chunker by class name (\p type).
        void addStreamChunker(QString type, QString name = QString());

        /// Adds a service chunker by class name (\p type).
        void addServiceChunker(QString type, QString name = QString());

        /// Returns the list of chunkers.
        const QSet<AbstractChunker* >& chunkers() const { return _chunkers; };

        /// Returns true if all the data chunkers are operating, otherwise false.
        bool isRunning() const;

    private:
        /// Add an allocated chunker.
        void _addChunker( AbstractChunker* chunker);

    private:
        FactoryConfig<AbstractChunker> *_factory;
        QMap<QPair<QString,quint16>,AbstractChunker* > _chunkerPortMap;
        QSet<AbstractChunker* > _chunkers;
        QSet<QString> _streamDataTypes;
        QSet<QString> _serviceDataTypes;
        QVector<DataReceiver*> _dataReceivers;
};

} // namespace pelican
#endif // CHUNKERMANAGER_H
