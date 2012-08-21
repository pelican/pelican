#include "pelican/comms/DataChunk.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/comms/ServerRequest.h"
#include "pelican/comms/ServerResponse.h"
#include "pelican/comms/AcknowledgementRequest.h"
#include "pelican/comms/DataSupportResponse.h"
#include "pelican/comms/DataSupportRequest.h"
#include "pelican/comms/ServiceDataRequest.h"
#include "pelican/comms/StreamDataRequest.h"
#include "pelican/comms/StreamData.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/data/DataBlob.h"
#include "pelican/data/DataBlobVerify.h"

#include <QtNetwork/QTcpSocket>
#include <QtCore/QDataStream>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QMapIterator>

#include <iostream>
using std::cout;
using std::endl;

namespace pelican {


PelicanProtocol::PelicanProtocol()
    : AbstractProtocol()
{
}


PelicanProtocol::~PelicanProtocol()
{
}



/**
 * @details
 * Creates a server request object based on the request received from the
 * PelicanServerClient using the PelicanClientProtocol
 */
boost::shared_ptr<ServerRequest> PelicanProtocol::request(QTcpSocket& socket)
{
    int timeout = 1000;
    ServerRequest::Request type = ServerRequest::Error;

    // If there are not enough bytes in the socket to hold the request type
    // return a error (bad) request.
    while (socket.bytesAvailable() < (int)sizeof(quint16))
    {
        if ( !socket.waitForReadyRead(timeout)) {
            return boost::shared_ptr<ServerRequest>(new ServerRequest(type,
                    socket.errorString()));
        }
    }

    // Read the request type and return an appropriate server request object.
    QDataStream in(&socket);
    quint16 tmp;
    in.setVersion(QDataStream::Qt_4_0);
    in >> tmp;
    type=(ServerRequest::Request)tmp;

    switch(type)
    {
        case ServerRequest::Acknowledge:
        {
            return boost::shared_ptr<AcknowledgementRequest>(
                    new AcknowledgementRequest());
            break;
        }

        case ServerRequest::ServiceData:
        {
            boost::shared_ptr<ServiceDataRequest> s(new ServiceDataRequest);
            quint16 num; /// \todo really good variable name!?
            in >> (quint16&)num;
            for(int i=0; i < num; ++i)
            {
                QString dtype;
                QString version;
                in >> dtype >> version;
                s->request(dtype,version);
            }
            return s;
        }

        case ServerRequest::StreamData:
        {
            boost::shared_ptr<StreamDataRequest> s(new StreamDataRequest);
            quint16 num;
            in >> num; /// \todo really good variable name!?
            for(int i = 0; i < num; ++i )
            {
                QSet<QString> serviceData;
                QSet<QString> streamData;
                in >> serviceData;
                in >> streamData;
                DataRequirements dr;
                dr.addServiceData(serviceData);
                dr.addStreamData(streamData);
                s->addDataOption(dr);
            }
            return s;
        }

        case ServerRequest::DataSupport:
        {
            boost::shared_ptr<DataSupportRequest> s(new DataSupportRequest);
            return s;
        }

        default:
            break;
    }
    return boost::shared_ptr<ServerRequest>(new ServerRequest(ServerRequest::Error, "PelicanProtocol: Unknown type passed"));
}


/**
 * @details
 */
void PelicanProtocol::send(QIODevice& device, const DataSupportResponse& supported)
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)ServerResponse::DataSupport;
    out << supported.streamData();
    out << supported.serviceData();
    device.write(array);
}


/**
 * @details
 */
void PelicanProtocol::send(QIODevice& stream, const AbstractProtocol::StreamData_t& data)
{
    // Write the stream data header consisting of:
    //
    // - Response type (Stream data in this case!)
    // - The Number of streams (data.size())
    //
    // For each stream data object in the stream data set.
    // - The stream data name, version id and size.
    // - The number of service data sets associated with the stream.
    // - For each service data its name, version id and size.

    // General header for the stream data set.
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)ServerResponse::StreamData;
    out << (quint16)data.size();
    QListIterator<StreamData*> i(data);

    // Header per stream data object.
    while (i.hasNext())
    {
        StreamData* sd = i.next();
        out << sd->name() << sd->id() << (quint64)(sd->size());

        // service data info
        out << (quint16) sd->associateData().size();
        foreach(const boost::shared_ptr<DataChunk>& dat, sd->associateData())
        {
            out << dat->name() << dat->id() << (quint64)(dat->size());
        }
    }

    stream.write(array);

    // Write the actual stream data.
    i.toFront();
    while (i.hasNext())
    {
        StreamData* sd = i.next();
        stream.write((const char*)sd->ptr(), sd->size());
        stream.waitForBytesWritten(-1);
    }
}


/**
 * @details
 */
void PelicanProtocol::send(QIODevice& stream, const AbstractProtocol::ServiceData_t& data )
{
    // construct the service data header
    // first integer is the number of Service Data sets
    // for each set there is a name tag, a version tag and size of data
    // followed by the binary data itself
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);

    // Write (send) the header.
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)ServerResponse::ServiceData;
    out << (quint16)data.size();

    QListIterator<DataChunk*> i(data);
    while (i.hasNext()) {
        DataChunk* d = i.next();
        out << d->name() << d->id() << (quint64)d->size();
    }
    stream.write(array);

    // Write (send) binary data.
    i.toFront();
    while (i.hasNext())
    {
        DataChunk* d = i.next();
        stream.write( (const char*)d->ptr(), (quint64)d->size() );
    }
}


/**
 * @details
 */
void PelicanProtocol::send(QIODevice& device, const QString& name, const DataBlob& data)
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)ServerResponse::Blob;
    out << data.type();
    out << name;
    out << data.serialisedBytes();
    if (device.write(array) < 0)
        throw QString("PelicanProtocol::send: Unable to write.");
    data.serialise(device);
#ifndef NDEBUG
    // Sanity check for debug mode
    // as this can cause massive headaches
    DataBlobVerify dbv( &data );
    if( ! dbv.verifySerialisedBytes() ) {
        QString msg( "PelicanProtocol: DataBlob ");
        msg += data.type() + " inconsistent serialisedBytes() method";
        std::cerr << msg.toStdString();
        throw( msg );
    }
#endif
}


/**
 * @details
 */
void PelicanProtocol::send(QIODevice& device, const QString& msg)
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << msg;
    out.device()->seek(0);
    out << (quint16)(array.size() - sizeof(quint16));
    device.write(array);
}


/**
 * @Details
 */
void PelicanProtocol::sendError(QIODevice& stream, const QString& msg)
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << msg;
    out.device()->seek(0);
    out << (quint16)(stream.size() - sizeof(quint16));
    stream.write(array);
}

} // namespace pelican
