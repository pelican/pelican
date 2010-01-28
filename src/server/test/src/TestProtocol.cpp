#include "TestProtocol.h"
#include "data/ServerRequest.h"

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

ServerRequest TestProtocol::request(QTcpSocket& socket)
{
    QStringList tokens;
    tokens.append(_id);
    return ServerRequest(ServerRequest::Acknowledge);
}

void TestProtocol::send( QByteArray& stream, const QString& message)
{
    stream.append(msg);
    stream.append(message);
    _last = stream;
}

void TestProtocol::send( QByteArray& stream, const AbstractProtocol::StreamData_t& )
{
    _last = stream;
}

void TestProtocol::send( QByteArray& stream, const AbstractProtocol::ServiceData_t& )
{
    _last = stream;
}

void TestProtocol::sendError( QByteArray& stream, const QString& msg)
{
    stream.append(error);
    stream.append(msg);
    _last = stream;
}

} // namespace pelican
