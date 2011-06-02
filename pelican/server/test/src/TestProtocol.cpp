#include <QtCore?QDataStream>
#include "TestProtocol.h"
#include "pelican/comms/StreamData.h"
#include "pelican/comms/ServerRequest.h"
#include "pelican/comms/StreamDataRequest.h"

#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {
namespace test {

// class TestProtocol
TestProtocol::TestProtocol(const QString& id, ServerRequest::Request request)
    : AbstractProtocol(), _id(id), _request(request)
{
}

TestProtocol::~TestProtocol()
{
}

QByteArray& TestProtocol::lastBlock()
{
    return _last;
}

boost::shared_ptr<ServerRequest> TestProtocol::request(QTcpSocket& )
{
    _last.clear();
    if (_request == ServerRequest::Acknowledge)
        return boost::shared_ptr<ServerRequest>(new ServerRequest(_request));
    else if (_request == ServerRequest::StreamData) {
        boost::shared_ptr<StreamDataRequest> ptr(new StreamDataRequest);
        DataRequirements req;
        req.addStreamData(_id);
        ptr->addDataOption(req);
        return static_cast<boost::shared_ptr<ServerRequest> >(ptr);
    } else if (_request == ServerRequest::ServiceData)
        return boost::shared_ptr<ServerRequest>(new ServerRequest(_request));

    return boost::shared_ptr<ServerRequest>(new ServerRequest(_request));
}

void TestProtocol::send( QIODevice& device, const QString& message)
{
    _last.clear();
    _last.append(message);
    device.write(_last);
}

void TestProtocol::send(QIODevice& device, const AbstractProtocol::StreamData_t& d)
{
    std::cout << "TestProtocol::send(): List length: " << d.size() << std::endl;
    Q_ASSERT(d.size() > 0);
    _last.clear();
    char* data = static_cast<char*>(d[0]->ptr());
    size_t size = d[0]->size() / sizeof(double);
    for (unsigned i = 0; i < size; i++) {
        std::cout << reinterpret_cast<double*>(data)[i];
    }
    std::cout << std::endl;
    _last.append(data);
    _lastStreamData = d;
    QDataStream stream(&device);
    stream.setVersion(QDataStream::Qt_4_0);
    stream << (quint16)_request;
    std::cout << "Sending request type " << (qint16)_request << std::endl;
    stream << _last.data();
}

void TestProtocol::send(QIODevice& device, const AbstractProtocol::ServiceData_t&)
{
    _last.clear();
    device.write(_last);
}

void TestProtocol::sendError( QIODevice& device, const QString& msg)
{
    _last.clear();
    _last.append(error);
    _last.append(msg);
    device.write(_last);
}

} // namespace test
} // namespace pelican
