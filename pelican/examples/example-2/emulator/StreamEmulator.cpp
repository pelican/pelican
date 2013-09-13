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


/*
 * This StreamEmulator sends data using a TCP socket.
 *
 *
 * Configuration
 * -----------------------------------------------------------------------------
 * It is configured by the following XML (populated here with example values)
 *
 * <StreamEmulator>
 *    <connection host="127.0.0.1" port="2001" />
 *    <packet     numSamples="100" />
 *    <data       numPackets="2000" pacetInterval="10" reportInterval="200" />
 * </SteamEmulator>
 *
 * The values of the host and port attributes determine the address on which
 * the TCP socket used to send emulated data should be opened.
 *
 * The numSamples attribute of packet determines the number of 4 byte values
 * are included in each emulated packets data section.
 *
 * The numPackets attribute determines the number of packets to be sent
 * by the emulator binary before it completes. If set to a value of "-1"
 * the emulator will continue sending packets until it is terminated by the
 * user.
 *
 * The packetInterval attribute is the delay between the sending of packets in
 * microseconds.
 *
 * The reportInterval is the interval between benchmark report messages and
 * is specified in numbers of packets.
 *
 * Packet format
 * -----------------------------------------------------------------------------
 * Emulated packets consist of a header and data section. The data section
 * consists of a number of 32 bit samples (qunit32 values), with the number
 * specified in the XML configuration. The header consists of the following:
 *
 *  - The total packet size, in bytes (4 bytes)
 *  - The packet counter. Index of the packet, starting at 0. (4bytes)
 *  - Timer value. Time since the last report log, in milliseconds. (4bytes)
 *  - Number of packets to be send by the emulator (4 bytes)
 *  - Report interval. Number of packets between report logs (4 bytes)
 *
 *  The header size is therefore 20 bytes.
 *
 *  The total size of the packet is therefore:
 *      20 bytes + number of samples x 4 bytes
 *
 * Note on the TCP socket connection
 * ----------------------------------------------------------------------------
 * The TCP socket, on which the emulator sends its data is instantiated in the
 * createDevice() method. If a connection cannot be made with the TCP server
 * immediately this function will block for up to 10 seconds repeatedly trying
 * to connect to the server.
 */


StreamEmulator::StreamEmulator(const pelican::ConfigNode& config)
: AbstractEmulator(), packetCounter_(0), reportCounter_(0)
{
    host_ = QHostAddress(config.getOption("connection", "host", "127.0.0.1"));
    port_ = config.getOption("connection", "port", "127.0.0.1").toShort();
    numSamples_ = (quint32)config.getOption("packet", "numSamples", "10000").toUInt();
    numPackets_ = config.getOption("data", "numPackets", "2000").toInt();
    packetInterval_ = config.getOption("data", "packetInterval", "0").toUInt();
    reportInterval_ = config.getOption("data", "reportInterval", QString::number(numPackets_)).toInt();

    headerSize_=3*sizeof(quint32)+2*sizeof(qint32);
    packetSize_=numSamples_*sizeof(quint32)+headerSize_;

    cout << "Emulator Settings:" << endl;
    cout << " * host            = " << host_.toString().toStdString() << endl;
    cout << " * port            = " << port_ << endl;
    cout << " * packet samples  = " << numSamples_ << endl;
    cout << " * packet size     = " << packetSize_ << " bytes ";
    cout << "[" <<packetSize_/(1024.0*1024.0) << " MiB]" << endl;
    cout << "   - header        = " << headerSize_ << " bytes" << endl;
    cout << "   - data          = " << packetSize_-headerSize_ << " bytes" << endl;
    cout << " * no. packets     = " << numPackets_ << endl;
    cout << " * report interval = " << reportInterval_ << endl;
    cout << " * packet interval = " << packetInterval_ << endl;

    // Resize the packet data.
    packet_.resize(packetSize_);
}

StreamEmulator::~StreamEmulator()
{
}

// Function called to obtain packet data.
// This function is called each time a new packet is required and is responsible
// for setting the data into the packet to be send.
//
// The function first updates the packet header and then sets the packet data
// (currently disabled for performance reasons).
//
// The data pointer of returned by this function is then written to the TCP
// socket by the emulator driver.
void StreamEmulator::getPacketData(char*& data, unsigned long& bytes)
{
    if (packetCounter_ == 0)
        timer_.start();

    data = packet_.data();
    bytes = packetSize_;

    // Update the packet header.
    quint32* header1 = reinterpret_cast<quint32*>(data);
    header1[0] = packetSize_;
    header1[1] = packetCounter_;
    header1[2] = timer_.elapsed();
    qint32* header2 = reinterpret_cast<qint32*>(data + 3*sizeof(quint32));
    header2[0] = numPackets_;
    header2[1] = reportInterval_;

    // Fill data section.
    // (Note: this is a major bottleneck therefore disabled for benchmark)
#if 0
    quint32* values = reinterpret_cast<quint32*>(data + headerSize_);
    for (size_t i = 0; i < numSamples_; ++i) {
        values[i] = (packetCounter_ * numSamples_) + i;
    }
#endif

    if (packetCounter_ > 0 && packetCounter_%reportInterval_ == 0) {
        printReport();
        timer_.restart();
    }

    ++packetCounter_; // Increment the packet counter.
}

// Interval between sending packets in microseconds.
unsigned long StreamEmulator::interval()
{
    return packetInterval_;
}

// Number of packets to send before terminating the emulator.
int StreamEmulator::nPackets()
{
    return numPackets_;
}

void StreamEmulator::emulationFinished()
{
    printReport();
}

void StreamEmulator::printReport()
{
    const double B2MiB = 1.0/(1024.0*1024.0);
    int elapsed = timer_.elapsed();
    size_t dataSent = packetSize_ * reportInterval_;
    char prefix[10];
    int l = (reportCounter_ < 10) ? 2 : ((reportCounter_ < 100) ? 1 : 0);
    sprintf(prefix, "E[%s%d] ", string(l, '-').c_str(), reportCounter_);
    printf("%s\n", string(80,'*').c_str());
    printf("%sPacket count  = %i", prefix, packetCounter_);
    if (numPackets_ > 0) printf(" of %i\n", numPackets_);
    else printf("\n");
    printf("%sPacket size   = %-7.3f MiB [%lu B]\n", prefix,
            packetSize_*B2MiB, packetSize_);
    printf("%sTotal sent    = %.1f MiB\n", prefix,
            packetSize_*packetCounter_*B2MiB);
    printf("%s\n", prefix);
    printf("%sReport interval:\n", prefix);
    printf("%s* Packets sent = %i\n", prefix, reportInterval_);
    printf("%s* Data sent    = %-7.3f MiB [%lu B]\n", prefix,
            dataSent*B2MiB, dataSent);
    printf("%s* Time taken   = %.3f s\n", prefix, elapsed*1.0e-3);
    printf("%s* Data rate    = ", prefix);
    if (elapsed > 0)
        printf("%-7.3f MiB/s\n", (dataSent*B2MiB)/(elapsed*1.0e-3));
    else
        printf("---     MiB/s\n");
    printf("%s\n\n", string(80,'*').c_str());
    fflush(stdout);
    reportCounter_++;
}


/**
 * Method to create the TCP device which is used to send emulated data.
 *
 * To connect the TCP socket requires that a TCP server has been created.
 * In this example the server is created in the StreamChunker in the server.
 *
 * This function therefore blocks, with a timeout of 10 seconds, repeatedly
 * trying to connect to the server every 0.1 seconds.
 *
 * If, after 10 seconds, a connection can't be established an exception is
 * thrown causing the emulator binary to terminate.
 */
QIODevice* StreamEmulator::createDevice()
{
    cout << endl;
    QTcpSocket* socket = new QTcpSocket();
    socket->abort();
    socket->connectToHost(host_, port_);

    // Block until the device can be connected...
    // TODO make timeout XML configurable.
    int maxTries = 100;
    int tryCount =  0;
    int delay = 1e5;  // 0.1 seconds
    cout << "Attempting to connect to server [timeout = "
         << delay/1e6 * maxTries << "s]\n..." << flush;
    while (!socket->waitForConnected(-1) && tryCount <= maxTries) {
        socket->connectToHost(host_, port_);
        usleep(delay);
        if (tryCount%5 == 0)
            cout << "." << flush;
        tryCount++;
    }
    if (!socket->waitForConnected(-1)) {
        throw QString("\nERROR: StreamEmulator::createDevice(): Timed out.  ") +
            QString(socket->errorString()) + QString(".");
    }
    else {
        cout << " Connected!" <<endl << endl;
    }

    return socket;
}

