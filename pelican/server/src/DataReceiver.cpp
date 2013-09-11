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

#define USE_SIGNALS 1

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
    // Setting active to false marks the destructor disconnect.
    _active = false;

    _chunker->stop();
    if (isRunning()) {
        do quit();
        while (!wait(10)); // {terminate();}
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


    if (_device) {
        // process any existing data on the stream
        if (_device->bytesAvailable() > 0) {
            _processIncomingData();
        }
    }

    // At this point the event loop takes over to call the chunker
    // next method
#if 0
    cout << "DataReceiver starting event loop for chunker. "
         << "(" << _chunker->chunkTypes()[0].toStdString() << ", "
         << _chunker->name().toStdString() << ")" << endl;
#endif

#if USE_SIGNALS
    // enter main event loop (until quit() is called)

    exec();
#else
    while (_active) {
        //cout << "DataReceiver: checking for data ... " << endl;

        if (!_device) {
            cerr << "DataReceiver: invalid device pointer!" << endl;
            _active = false;
            break;
        }

        if (_device->bytesAvailable() > 0) {
            _chunker->next(_device);
        }

        // If the device is a socket handle error conditions
        if (QAbstractSocket* s = dynamic_cast<QAbstractSocket*>(_device)) {
//            if (s->error())
//                _processError();
            if (s->state() == QAbstractSocket::UnconnectedState)
                _reconnect();
        }
        // For devices with no reconnect exit the event loop if the
        // device becomes unreadable.
        else if (!_device->isReadable()) {
            cerr << "DataReceiver: device not readable!" << endl;
            _active = false;
            break;
        }

        usleep(1); // how long should this be.
    }
#endif
}

// NOTE this function is only called if using signals.
//
void DataReceiver::_processIncomingData()
{
    _chunker->next(_device);
    if (_device && _device->bytesAvailable() > 0) {
        emit dataRemaining();
    }
}


void DataReceiver::_processError() {
    std::cerr << "DataReceiver: socket error: " <<
        _device->errorString().toStdString() << std::endl;
    if (_active) _reconnect();
}


void DataReceiver::_reconnect() {
    std::cerr << "DataReceiver Attempting to reconnect." << std::endl;
    _deleteDevice();
    if (_active)
        _setupDevice();
}


void DataReceiver::_setupDevice()
{
    _device = _chunker->newDevice();
    _chunker->activate();

#if USE_SIGNALS
    if (_device) {
        // Call the chunker->next() method if there is data to process.
        connect(_device, SIGNAL(readyRead()), SLOT(_processIncomingData()),
                Qt::DirectConnection);
        connect(this, SIGNAL(dataRemaining()), SLOT(_processIncomingData()),
                Qt::DirectConnection);

        // If its a socket we must try and catch the failure conditions
        if (QAbstractSocket* s = dynamic_cast<QAbstractSocket*>(_device)) {
            _registerSocket(s);
        }
    }
#endif
}

// NOTE this function is not called if not using signals
void DataReceiver::_registerSocket(QAbstractSocket* socket)
{
    Q_ASSERT(USE_SIGNALS);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
             SLOT(_processError()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()),
             SLOT(_reconnect()), Qt::DirectConnection);
}

void DataReceiver::_deleteDevice()
{
    _chunker->stop();       // TODO is this only if the chunker writer respects
                            // the AbstractChunker::_active attribute ?
    _device->disconnect();  // Disconnects all signals in this object from
                            // receiver's method.
    _device->deleteLater(); // Note: deleteLater() acts on the current pointer
                            // (i.e. takes a copy of the current pointer for
                            // deletion) therefore it is safe to set to null
                            // or reassign immediately
    _device = 0;
}

} // namespace pelican
