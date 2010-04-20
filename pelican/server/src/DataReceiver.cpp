#include "pelican/server/DataReceiver.h"
#include "pelican/utility/pelicanTimer.h"
#include <QIODevice>
#include <QTimer>
#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * DataReceiver constructor.
 */
DataReceiver::DataReceiver(AbstractChunker* chunker) :
    QThread(), _chunker(chunker), _device(0)
{
    _device = 0;
    if (!chunker)
        throw QString("DataReceiver: Invalid chunker.");
}

/**
 * @details
 * DataReceiver destructor. This ensures that the thread has finished
 * executing (calling quit() if necessary) before returning.
 */
DataReceiver::~DataReceiver()
{
    // If the thread has started, call quit() to exit the event loop.
    if (isRunning()) while (!isFinished()) quit();
    wait();
}

/**
 * @details
 * Runs the thread owned by the DataReceiver object.
 * The thread creates and opens the socket and connects the readyRead()
 * socket signal to the processIncomingData() slot, before entering its
 * own event loop.
 */
void DataReceiver::run()
{
    // Open up the device to use.
    _device = _chunker->newDevice();
    _chunker->setDevice(_device);
    if (_device) {
        connect(_device, SIGNAL(readyRead()), SLOT(processIncomingData()));
        exec(); // Enter event loop here.
    }
}

} // namespace pelican
