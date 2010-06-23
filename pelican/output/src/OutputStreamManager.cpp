#include "OutputStreamManager.h"
#include "pelican/utility/memCheck.h"

namespace pelican {

class Config;


/**
 *@details OutputStreamManager 
 */
OutputStreamManager::OutputStreamManager( const Config* config , const Config::TreeAddress& base )
{
    // facotry to allow us to generate streamers from the XML
    //_factory = new FactoryConfig<AbstractOutputStream>(config, base);
}

/**
 *@details
 */
OutputStreamManager::~OutputStreamManager()
{
    //delete _factory;
}

/**
 * @details
 * adds a output stream plugin that will be clled each time data for a specific stream arrives
 */
void OutputStreamManager::addOutputStream(const QString& stream, AbstractOutputStream* streamer)
{
    _streamers[stream].append(streamer);
}

void OutputStreamManager::send( DataBlob* data, const QString& stream )
{
    if( _streamers.contains(stream) ) {
        foreach( AbstractOutputStream* out, _streamers[stream]) {
            out->send(stream, data);
        }
    }
}

} // namespace pelican
