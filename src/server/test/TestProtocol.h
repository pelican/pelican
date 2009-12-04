#ifndef TESTPROTOCOL_H
#define TESTPROTOCOL_H
#include <QString>

#include "AbstractProtocol.h"

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
    public:
        TestProtocol(const QString& id);
        ~TestProtocol();
        virtual ServerRequest request(QTcpSocket& socket);
        virtual void send( QDataStream& stream, const QList<DataBlob>& );
        virtual void send( QDataStream& stream, const QString& message );
        virtual void sendError( QDataStream& stream, const QString&);

    private:
        QString _id;
};

} // namespace pelican
#endif // TESTPROTOCOL_H 
