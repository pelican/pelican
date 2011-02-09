#ifndef TESTCHUNKER_H
#define TESTCHUNKER_H

/**
 * @file TestChunker.h
 */

#include "pelican/server/AbstractChunker.h"
#include "pelican/utility/Config.h"

#include <QtCore/QString>
#include <QtCore/QThread>

#include <cstring>

class QTimer;

namespace pelican {
class DataManager;
namespace test {


/**
 * @ingroup t_server
 *
 * @class TestChunker
 *
 * @brief
 * Implementation of an AbstractChunker to monitor calling.
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
                size_t size = 0, const QString& host = "", quint16 port = 0,
                QObject* parent = 0);

        /// Constructs a new TestChunker.
        TestChunker(const ConfigNode& config);

        /// Destroys the TestChunker.
        ~TestChunker();

        /// Creates the socket to use for the incoming data stream.
        virtual QIODevice* newDevice();

        /// Gets the next chunk of data.
        virtual void next(QIODevice*);

    protected:
        /// Runs the thread owned by the test chunker.
        void run();

    private:
        bool _abort;
        QIODevice* _device;
        bool _badSocket;
        size_t _size;
};

PELICAN_DECLARE_CHUNKER(TestChunker)

} // namespace test
} // namespace pelican
#endif // TESTCHUNKER_H
