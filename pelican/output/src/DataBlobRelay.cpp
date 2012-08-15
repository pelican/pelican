#include "DataBlobRelay.h"
#include "pelican/utility/Config.h"
#include "pelican/output/Stream.h"
#include "pelican/output/DataBlobClient.h"
#include "pelican/output/OutputStreamManager.h"
#include <iostream>


namespace pelican {

/**
 *@details DataBlobRelay 
 */
DataBlobRelay::DataBlobRelay( const Config* config, const Config::TreeAddress& address )
{
    ConfigNode localConfig = config->get( address );
    // setup output manager
    Config::TreeAddress outputAddress = address;
    outputAddress << Config::NodeId("output","");
    _outputManager = new OutputStreamManager( config, outputAddress );

    // setup clients
    foreach( const ConfigNode& node, localConfig.getNodes("client")   ) {
        DataBlobClient* c = new DataBlobClient( node );
        _myClients.append( c );
        addClient( c );
    }
}

/**
 *@details
 */
DataBlobRelay::~DataBlobRelay()
{
    foreach( DataBlobClient* client, _myClients ) {
        delete client;
    }
    delete _outputManager;
}

void DataBlobRelay::connectToStream( AbstractOutputStream* streamer, const QString& stream) {
    _outputManager->connectToStream( streamer, stream );
}

void DataBlobRelay::addClient( AbstractDataBlobClient* client ) 
{
    _clients.append(client);
    connect( client, SIGNAL( newData(const Stream&) ), 
             this, SLOT( _streamData( const Stream& ) ) );
} 

void DataBlobRelay::_streamData( const Stream& s ) {
    boost::shared_ptr<DataBlob> data = s.data();
    _outputManager->send( data.get() , s.name() );
}

} // namespace pelican
