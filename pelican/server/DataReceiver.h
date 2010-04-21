#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include "pelican/server/AbstractChunker.h"

#include <QThread>
#include <QString>

class QIODevice;

/**
 * @file DataReceiver.h
 */

namespace pelican {

class DataManager;

/**
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
        AbstractChunker* _chunker;
        QIODevice* _device; // This is usually a UDP socket.

    public:
        /// Constructor.
        DataReceiver(AbstractChunker* chunker);

        /// Destructor.
        ~DataReceiver();

        /// Connect to the specified host and listen for incoming data.
        void listen() {start();}

    protected slots:
        /// This slot is called when data is available on the device.
        void processIncomingData() {_chunker->next(_device);}

    protected:
        /// Runs the thread for the data receiver.
        void run();
};

} // namespace pelican

#endif // DATARECEIVER_H
