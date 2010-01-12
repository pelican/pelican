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

ServerRequest TestProtocol::request(QTcpSocket& socket)
{
    QStringList tokens;
    tokens.append(_id);
    return ServerRequest(ServerRequest::Acknowledge);
}

void TestProtocol::send( QDataStream& stream, const QString& msg )
{
    stream << _id;
}

void TestProtocol::send( QDataStream& stream, const AbstractProtocol::StreamData_t& )
{
}

void TestProtocol::send( QDataStream& stream, const AbstractProtocol::ServiceData_t& )
{
}

void TestProtocol::sendError( QDataStream& stream, const QString& msg)
{
    stream << "ERROR" << msg << "\n";
}

} // namespace pelican
