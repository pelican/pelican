#ifndef TESTCHUNKER_H
#define TESTCHUNKER_H

#include "pelican/server/AbstractChunker.h"
#include "pelican/utility/Config.h"

#include <QString>
#include <QThread>
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
class TestChunker : public QThread, public AbstractChunker
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

    protected:
        /// Runs the thread owned by the test chunker.
        void run();

    private:
        bool _abort;
        QIODevice* _device;
        bool _badSocket;
        unsigned int _nextCount;
        size_t _size;
};

} // namespace pelican

#endif // TESTCHUNKER_H
