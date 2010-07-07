#include "pelican/server/DataReceiver.h"
#include "pelican/utility/pelicanTimer.h"
#include <QIODevice>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QFile>
#include <iostream>
//#include <sys/types.h>
//#include <sys/syscall.h>
//#include <unistd.h>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * DataReceiver constructor.
 */
DataReceiver::DataReceiver(AbstractChunker* chunker) :
        QThread(), _chunker(chunker), _device(0), _watcher(0)
{
    _abort = false;
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
    _abort = true;
    delete _watcher;
    _chunker->stop();
    do quit(); while (!wait(10));
}

/**
 * @details
 * Runs the thread owned by the DataReceiver object.
 * The thread creates and opens the socket and connects the readyRead()
 * socket signal to the private _processIncomingData() slot using a direct
 * connection, before entering its own event loop.
 */
void DataReceiver::run()
{
    if(  _chunker->watchFile() == "" )
    {
        // Open up the device to use.
        _device = _chunker->newDevice();
        _chunker->setDevice(_device);
        //    int tid = syscall(__NR_gettid);
        //    std::cout << "Thread ID : " << tid << std::endl;
        if (_device) {
            connect(_device, SIGNAL(readyRead()), SLOT(_processIncomingData()),
                    Qt::DirectConnection);
            exec();
        }
    }
    else {
        // start watching the specified file
        _watcher = new QFileSystemWatcher;
        _watcher->addPath( _chunker->watchFile() );
        connect( _watcher, SIGNAL(fileChanged ( const QString &) ),
                 this,  SLOT(_fileChanged(const QString &) ),
                 Qt::DirectConnection);
        exec();
        delete _watcher; _watcher = 0;
    }
}

void DataReceiver::_processIncomingData() 
{
    _chunker->next(_device);
}

void DataReceiver::_fileChanged(const QString & filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly )) return;
    _chunker->next(&file);
}

} // namespace pelican
