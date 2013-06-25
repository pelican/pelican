/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "server/DataReceiver.h"
#include "utility/pelicanTimer.h"
#include <QtCore/QIODevice>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <iostream>

using namespace std;

namespace pelican {

/**
 * @details
 * DataReceiver constructor.
 */
DataReceiver::DataReceiver(AbstractChunker* chunker)
: QThread(), _active(false), _chunker(chunker), _device(0)
{
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
    _active = false; // mark destructor disconnect
    _chunker->stop();
    if( isRunning() ) {
        do quit();
        while( !wait(10) );// { terminate(); }
    }
    delete _device;
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
    _active = true;

    // Open up the device to use.
    // N.B. must be done in this thread (i.e. in run())
    _setupDevice();

    //    int tid = syscall(__NR_gettid);
    //    std::cout << "Thread ID : " << tid << std::endl;
    if (_device) {
        // process any existing data on the stream
        if( _device->bytesAvailable() > 0 )
            _processIncomingData();
    }

    // enter main event loop (until quit() is called)
    exec();
}

void DataReceiver::_processIncomingData()
{
    _chunker->next(_device);
}

void DataReceiver::_registerSocket( QAbstractSocket* socket ) {
    connect( socket, SIGNAL( error(QAbstractSocket::SocketError) ),
             SLOT(_processError()), Qt::DirectConnection );
    connect( socket, SIGNAL( disconnected() ),
             SLOT(_reconnect()), Qt::DirectConnection);
}

void DataReceiver::_processError() {
    std::cerr << "DataReceiver: socket error: " <<
        _device->errorString().toStdString() << std::endl;
    if (_active) _reconnect();
}

void DataReceiver::_reconnect() {
    std::cerr << "DataReceiver: Attempting to reconnect.\n";
    _deleteDevice();
    _setupDevice();
}

void DataReceiver::_setupDevice()
{
    _device = _chunker->newDevice();
    _chunker->activate();

    if (_device) {
        // If its a socket we must try and catch the failure conditions
        connect(_device, SIGNAL(readyRead()), SLOT(_processIncomingData()),
                Qt::DirectConnection);
        if (QAbstractSocket* s = dynamic_cast<QAbstractSocket*>(_device)) {
            _registerSocket( s );
        }
    }
}


void DataReceiver::_deleteDevice()
{
    _chunker->stop();
    _device->disconnect();  // Disconnects all signals in this object from receiver's method.
    _device->deleteLater(); // Delete later acts on the current pointer
                            // (i.e. takes a copy of the current pointer for
                            // deletion)
                            // therefore it is safe to set to null or reassign
                            // immediately
    _device = 0;
}

} // namespace pelican
