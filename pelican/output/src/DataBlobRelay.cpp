#include "DataBlobRelay.h"
#include "pelican/utility/Config.h"
#include "pelican/output/Stream.h"
#include "pelican/output/DataBlobClient.h"
#include "pelican/output/OutputStreamManager.h"


namespace pelican {

/**
 *@details DataBlobRelay 
 */
DataBlobRelay::DataBlobRelay( const Config* config, const Config::TreeAddress& address )
{
    ConfigNode localConfig = config->get( address );
    // setup output manager
    Config::TreeAddress outputAddress = address;
    outputAddress << Config::NodeId("OutputManager","");
    _outputManager = new OutputStreamManager( config, outputAddress );

    // setup clients
    foreach( const ConfigNode& node, localConfig.getNodes("listen")   ) {
        addClient( new DataBlobClient( node ) );
    }
}

/**
 *@details
 */
DataBlobRelay::~DataBlobRelay()
{
    foreach( DataBlobClient* client, _clients ) {
        delete client;
    }
    delete _outputManager;
}

void DataBlobRelay::addClient( DataBlobClient* client ) 
{
    _clients.append(client);
    connect( client, SIGNAL( newData(const Stream&) ), 
             this, SLOT( _streamData() ) );
} 

void DataBlobRelay::_streamData( const Stream& s ) {
    boost::shared_ptr<DataBlob> data = s.data();
    _outputManager->send( data.get() , s.name() );
}

} // namespace pelican
