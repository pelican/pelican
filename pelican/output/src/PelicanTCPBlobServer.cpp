#include <QBuffer>
#include "PelicanTCPBlobServer.h"


#include "pelican/server/WritableData.h"
#include "pelican/server/DataManager.h"
#include "pelican/server/StreamDataBuffer.h"
#include "pelican/utility/memCheck.h"

#include <QCoreApplication>

namespace pelican {


/**
 *@details PelicanTCPBlobServer 
 */
PelicanTCPBlobServer::PelicanTCPBlobServer(QObject* parent)
    : AbstractBlobServer(parent)
{
    //_dataManger = new DataManager(config); TODO
    _tcpServer = new QTcpServer(this);

    // start TCP Server, which listens for incoming connections
    QHostAddress host = QHostAddress("127.0.0.1");
    if (!_tcpServer -> listen(host, 8888))
        throw QString("Unable to start QTcpServer: %1").arg(_tcpServer -> errorString()).toStdString();
    
    connect(_tcpServer, SIGNAL(newConnection()), this, SLOT(acceptClientConnection()));

    // start main thread
    start();
}

/**
 *@details
 */
PelicanTCPBlobServer::~PelicanTCPBlobServer()
{
    // Close tcpServer
    _tcpServer -> close();

    // Wait for main thread to finish.
    if (isRunning()) while (!isFinished()) quit();
    wait();
}

/**
 *@details
 */
void PelicanTCPBlobServer::acceptClientConnection() 
{
    QTcpSocket *client = _tcpServer -> nextPendingConnection();
    std::cout << "Client connected" << std::endl;
}

void PelicanTCPBlobServer::run()
{
 //   exec();

    // TODO
    /*
    while( _run ) {
        foreach(const QString& type, clientsRequestingData.keys() ) {
                LockedData d =_dataManger->getNext(type) ) {
                if( d.isValid() ) {
                    // send it out to all clients for this dat type
                    foreach(connectedClient, clientsRequestingData[type] )
                    {
                    }
                }
            }
        }
    }
    */
}

} // namespace pelican
