#ifndef TESTCHUNKER_H
#define TESTCHUNKER_H

#include "pelican/server/AbstractChunker.h"
#include "pelican/utility/Config.h"

#include <QString>
#include <QObject>
#include <cstring>

class QTimer;

/**
 * @file TestChunker.h
 */

namespace pelican {

class DataManager;

/**
 * @class TestChunker
 *
 * @brief
 * Implemnetation of an AbstractChunker to monitor calling.
 *
 * @details
 *
 */
class TestChunker : public QObject, public AbstractChunker
{
    Q_OBJECT

    public:
        /// Constructs a new TestChunker.
        TestChunker(const QString& type = "ChunkType", bool badSocket = false,
                size_t size = 0, QString host = "", quint16 port = 0,
                QObject* parent = 0);

        /// Constructs a new TestChunker.
        TestChunker(const ConfigNode& config);

        /// Destroys the TestChunker.
        ~TestChunker();

        /// Creates the socket to use for the incoming data stream.
        virtual QIODevice* newDevice();

        /// Gets the next chunk of data.
        virtual void next(QIODevice*);

        /// return the number of times that the next method has been called
        //  will also reset the counter
        unsigned int nextCalled();

    private slots:
        /// Starts the data stream.
        void _start();

    private:
        QIODevice* _device;
        QTimer* _timer;
        bool _badSocket;
        unsigned int _nextCount;
        size_t _size;
};

} // namespace pelican

#endif // TESTCHUNKER_H
