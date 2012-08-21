#ifndef TESTDATABLOBSERVER_H
#define TESTDATABLOBSERVER_H

#include <QtCore/QString>

/**
 * @file TestDataBlobServer.h
 */

namespace pelican {
class PelicanTCPBlobServer;
class DataBlob;
namespace test {

/**
 * @class TestDataBlobServer
 *  
 * @brief
 * 
 * @details
 * 
 */

class TestDataBlobServer
{
    public:
        TestDataBlobServer(  );
        ~TestDataBlobServer();
        QString host() const;
        quint16 port() const;
        void send(DataBlob*, const QString& stream = QString());
        int clientsForStream( const QString& stream ) const;

    private:
        PelicanTCPBlobServer* _server;
};

} // namespace test
} // namespace pelican
#endif // TESTDATABLOBSERVER_H 
