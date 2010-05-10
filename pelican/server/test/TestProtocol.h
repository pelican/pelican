#ifndef TESTPROTOCOL_H
#define TESTPROTOCOL_H

#include "pelican/comms/AbstractProtocol.h"
#include "pelican/comms/ServerRequest.h"
#include <QString>
#include <QIODevice>

/**
 * @file TestProtocol.h
 */

namespace pelican {

/**
 * @class TestProtocol
 *
 * @brief
 * A dummy protocol for testing.
 *
 * @details
 *
 */
class TestProtocol : public AbstractProtocol
{
    private:
        enum {ack, msg, error};

    public:
        TestProtocol(const QString& id, ServerRequest::Request request = ServerRequest::Acknowledge);
        ~TestProtocol();
        virtual boost::shared_ptr<ServerRequest> request(QTcpSocket& socket);
        virtual void send( QIODevice& device, const AbstractProtocol::StreamData_t& );
        virtual void send( QIODevice& device, const AbstractProtocol::ServiceData_t& );
        virtual void send( QIODevice& device, const QString& message );
        virtual void send( QIODevice& , const DataBlob& ) {};
        virtual void sendError( QIODevice& device, const QString&);

        QByteArray& lastBlock();
        AbstractProtocol::StreamData_t lastStreamData() const { return _lastStreamData; };

    private:
        AbstractProtocol::StreamData_t _lastStreamData;
        QByteArray _last;
        QString _id;
        ServerRequest::Request _request;
};

} // namespace pelican

#endif // TESTPROTOCOL_H
