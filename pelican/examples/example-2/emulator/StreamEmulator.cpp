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

#include "StreamEmulator.hpp"

#include <QtNetwork/QTcpSocket>

#include <iostream>
#include <stdexcept>
#include <string>

#include <unistd.h>

#include <QtCore/QTime>

using namespace std;

StreamEmulator::StreamEmulator(const pelican::ConfigNode& config)
: AbstractEmulator(), packetCounter_(0)
{
    host_ = QHostAddress(config.getOption("connection", "host", "127.0.0.1"));
    port_ = config.getOption("connection", "port", "127.0.0.1").toShort();
    numSamples_ = (quint32)config.getOption("packet", "numSamples", "10000").toUInt();
    numPackets_ = config.getOption("data", "numPackets", "2000").toInt();
    packetInterval_ = config.getOption("data", "packetInterval", "0").toUInt();
    reportInterval_ = config.getOption("data", "reportInterval", QString::number(numPackets_)).toInt();

    headerSize_=3*sizeof(quint32)+2*sizeof(qint32);
    packetSize_=numSamples_*sizeof(quint32)+headerSize_;

    cout << "StreamEmulator: host            = " << host_.toString().toStdString() << endl;
    cout << "StreamEmulator: port            = " << port_ << endl;
    cout << "StreamEmulator: packet samples  = " << numSamples_ << endl;
    cout << "StreamEmulator: packet size     = " << packetSize_ << " bytes" << endl;
    cout << "StreamEmulator: packet size     = " << packetSize_/(1024.0*1024.0) << " MiB" << endl;
    cout << "StreamEmulator: no. packets     = " << numPackets_ << endl;
    cout << "StreamEmulator: report interval = " << reportInterval_ << endl;
    cout << "StreamEmulator: packet interval = " << packetInterval_ << endl;

    // Resize the packet data.
    packet_.resize(packetSize_);
}

StreamEmulator::~StreamEmulator()
{
}

// Function called to obtain packet data.
void StreamEmulator::getPacketData(char*& data, unsigned long& bytes)
{
    if (packetCounter_ == 0)
        timer_.start();

    data = packet_.data();
    bytes = packetSize_;

    // Update the packet header.
    *reinterpret_cast<quint32*>(data+0*sizeof(quint32)) = packetSize_;
    *reinterpret_cast<quint32*>(data+1*sizeof(quint32)) = packetCounter_;
    *reinterpret_cast<quint32*>(data+2*sizeof(quint32)) = timer_.elapsed();
    *reinterpret_cast<qint32*>(data+3*sizeof(quint32))   = numPackets_;
    *reinterpret_cast<qint32*>(data+3*sizeof(quint32)+sizeof(qint32)) = reportInterval_;

    // Fill data section.
    // (Note: this is a major bottleneck therefore disabled for benchmark)
#if 0
    quint32* values = reinterpret_cast<quint32*>(data + headerSize_);
    for (size_t i = 0; i < numSamples_; ++i) {
        values[i] = (packetCounter_ * numSamples_) + i;
    }
#endif

//    cout << packetCounter_ << endl;
    if (packetCounter_ > 0 && packetCounter_%reportInterval_ == 0) {
        emulationFinished();
        timer_.restart();
    }

    ++packetCounter_; // Increment the packet counter.
}

unsigned long StreamEmulator::interval()
{
    return packetInterval_;
}

int StreamEmulator::nPackets()
{
    return numPackets_;
}

void StreamEmulator::emulationFinished()
{
    float elapsed = timer_.elapsed() / 1.0e3;
    float MiB = (packetSize_ * reportInterval_) / (1024.0 * 1024.0);
    cout << endl;
    cout << string(80, '*') << endl;
    cout << __PRETTY_FUNCTION__ << endl;
    cout << std::string(80, '-') << endl;
    cout << "-- packet count = " << packetCounter_ << endl;
    cout << "-- no. packets  = " << numPackets_ << endl;
    cout << "-- packet size  = " << packetSize_ << " bytes" << endl;
    cout << "-- packet size  = " << packetSize_/(1024.0*1024.0) << " MiB" << endl;
    cout << "-- time taken   = " << elapsed << " seconds." << endl;
    cout << "-- MiB (sent)   = " << MiB << std::endl;
    cout << "-- MiB/s        = " << MiB/elapsed  << endl;
    cout << std::string(80, '*') << endl;
    cout << endl;
}

QIODevice* StreamEmulator::createDevice()
{
    cout << endl;
    QTcpSocket* socket = new QTcpSocket();
    socket->abort();
    socket->connectToHost(host_, port_);

    // Block until the device can be connected...
    int maxTries = 100;
    int tryCount =  0;
    while (!socket->waitForConnected(-1) && tryCount <= maxTries) {
        socket->connectToHost(host_, port_);
        usleep(1e5); // 0.1 seconds
        tryCount++;
    }

    if (!socket->waitForConnected(-1)) {
        throw QString("\n[[ StreamEmulator::createDevice(): Timed out.  ") +
            QString(socket->errorString()) + QString(". ]]");
    }

    cout << "=== Emulator connected to server on:\n";
    cout << "    host = " << host_.toString().toStdString() << endl;
    cout << "    port = " << port_ << endl;
    cout << endl;
    return socket;
}

