#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include "pelican/server/AbstractChunker.h"

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QString>

class QIODevice;

/**
 * @file DataReceiver.h
 */

namespace pelican {

class DataManager;

/**
 * @ingroup c_server
 *
 * @class DataReceiver
 *
 * @brief
 * Connects incoming data streams to the chunker inside a separate thread.
 *
 * @details
 *
 */
class DataReceiver : public QThread
{
    Q_OBJECT

    private:
        bool _abort;
        QMutex _mutex;
        AbstractChunker* _chunker;
        QIODevice* _device; // This is usually a UDP socket.

    public:
        /// Constructor.
        DataReceiver(AbstractChunker* chunker);

        /// Destructor.
        ~DataReceiver();

        /// Connect to the specified host and listen for incoming data.
        void listen() {start();}

    private slots:
        /// This slot is called when data is available on the device.
        void _processIncomingData();

    protected:
        /// Runs the thread for the data receiver.
        void run();
};

} // namespace pelican

#endif // DATARECEIVER_H
