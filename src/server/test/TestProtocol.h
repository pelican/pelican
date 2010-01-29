#ifndef TESTPROTOCOL_H
#define TESTPROTOCOL_H
#include <QString>

#include "comms/AbstractProtocol.h"
#include <QByteArray>

/**
 * @file TestProtocol.h
 */

namespace pelican {

/**
 * @class TestProtocol
 *  
 * @brief
 *     A dummy protocol for testing
 * @details
 * 
 */

class TestProtocol : public AbstractProtocol
{
        enum sendType_t {ack, msg,error};

    public:
        TestProtocol(const QString& id);
        ~TestProtocol();
        virtual ServerRequest request(QTcpSocket& socket);
        virtual void send( QByteArray& stream, const AbstractProtocol::StreamData_t& );
        virtual void send( QByteArray& stream, const AbstractProtocol::ServiceData_t& );
        virtual void send( QByteArray& stream, const QString& message );
        virtual void sendError( QByteArray& stream, const QString&);

        QByteArray& lastBlock();

    private:
        QByteArray _last;
        QString _id;
};

} // namespace pelican
#endif // TESTPROTOCOL_H 
