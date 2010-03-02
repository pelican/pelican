#ifndef TESTPROTOCOL_H
#define TESTPROTOCOL_H
#include <QString>

#include "comms/AbstractProtocol.h"
#include <QIODevice>

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
        virtual boost::shared_ptr<ServerRequest> request(QTcpSocket& socket);
        virtual void send( QIODevice& stream, const AbstractProtocol::StreamData_t& );
        virtual void send( QIODevice& stream, const AbstractProtocol::ServiceData_t& );
        virtual void send( QIODevice& stream, const QString& message );
        virtual void sendError( QIODevice& stream, const QString&);

        QByteArray& lastBlock();

    private:
        QByteArray _last;
        QString _id;
};

} // namespace pelican
#endif // TESTPROTOCOL_H 
