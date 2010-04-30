#include <QBuffer>
#include "PelicanTCPBlobServer.h"


#include "pelican/server/WritableData.h"
#include "pelican/server/DataManager.h"
#include "pelican/server/StreamDataBuffer.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/memCheck.h"

#include <QCoreApplication>

namespace pelican {


/**
 *@details PelicanTCPBlobServer 
 */
PelicanTCPBlobServer::PelicanTCPBlobServer(const ConfigNode& config, QObject* parent)
//    : AbstractBlobServer(parent)
      : QThread(parent), _port(0)
{
    int port = config.getOption("connection", "port").toInt();
    if( port > 0 )
    {
        _port = port;
    }
    // start main thread
    start();
}

/**
 *@details
 */
PelicanTCPBlobServer::~PelicanTCPBlobServer()
{
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
    //_dataManger = new DataManager(config); TODO
    _tcpServer = new QTcpServer;

    // start TCP Server, which listens for incoming connections
    if (!_tcpServer -> listen( QHostAddress::Any, _port))
    {
        std::cerr << QString("Unable to start QTcpServer: %1").arg( _tcpServer->errorString()).toStdString();
    }
    else {
        connect(_tcpServer, SIGNAL(newConnection()), this, SLOT(acceptClientConnection()), Qt::DirectConnection );
        exec();
    }

    // cleanup
    delete _tcpServer;

    // TODO
    // Actually maybe we could just do this with callbacks?
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
