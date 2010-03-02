#include "TestProtocol.h"
#include "comms/ServerRequest.h"

#include <iostream>
#include "utility/memCheck.h"

namespace pelican {

// class TestProtocol 
TestProtocol::TestProtocol(const QString& id)
    : AbstractProtocol(), _id(id)
{
}

TestProtocol::~TestProtocol()
{
}

QByteArray& TestProtocol::lastBlock()
{
    return _last;
}

boost::shared_ptr<ServerRequest> TestProtocol::request(QTcpSocket& socket)
{
    _last.clear();
    return boost::shared_ptr<ServerRequest>(new ServerRequest(ServerRequest::Acknowledge));
}

void TestProtocol::send( QIODevice& stream, const QString& message)
{
    _last.clear();
    _last.append(message);
    stream.write(_last);
}

void TestProtocol::send( QIODevice& stream, const AbstractProtocol::StreamData_t& )
{
    _last.clear();
    stream.write(_last);
}

void TestProtocol::send( QIODevice& stream, const AbstractProtocol::ServiceData_t& )
{
    _last.clear();
    stream.write(_last);
}

void TestProtocol::sendError( QIODevice& stream, const QString& msg)
{
    _last.clear();
    _last.append(error);
    _last.append(msg);
    stream.write(_last);
}

} // namespace pelican
