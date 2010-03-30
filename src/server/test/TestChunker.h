#ifndef TESTCHUNKER_H
#define TESTCHUNKER_H
#include <QString>
#include <QObject>
#include <cstring>
#include "server/AbstractChunker.h"
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
        TestChunker(const QString& type = "ChunkType",
                bool badSocket = false, size_t size = 0, QString host = "",
                quint16 port = 0, QObject* parent = 0);

        /// Destroys the TestChunker.
        ~TestChunker();

        /// Creates the socket to use for the incoming data stream.
        virtual QIODevice* newDevice();

        ///
        virtual void next(QIODevice*);

        /// return the number of times that the next method has been called
        //  will also reset the counter
        unsigned int nextCalled();

    private slots:
        /// Starts the data stream.
        void _start();

    private:
        QTimer* _timer;
        bool _badSocket;
        unsigned int _nextCount;
        size_t _size;
};

} // namespace pelican

#endif // TESTCHUNKER_H 
