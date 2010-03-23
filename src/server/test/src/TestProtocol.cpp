#include "TestProtocol.h"
#include "comms/ServerRequest.h"
#include "comms/StreamDataRequest.h"

#include <iostream>
#include "utility/memCheck.h"

namespace pelican {

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
        req.addStreamData("TestStream");
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

void TestProtocol::send( QIODevice& device, const AbstractProtocol::StreamData_t& d)
{
    std::cout << "TestProtocol::send()" << std::endl;
    _last.clear();
    _lastStreamData = d;
    device.write(_last);
}

void TestProtocol::send( QIODevice& device, const AbstractProtocol::ServiceData_t& )
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

} // namespace pelican
