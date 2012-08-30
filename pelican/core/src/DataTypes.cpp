#include "pelican/core/DataTypes.h"
#include "pelican/core/AbstractStreamAdapter.h"
#include "pelican/core/AbstractServiceAdapter.h"
#include "pelican/core/DataClientFactory.h"
#include "pelican/data/DataSpec.h"


namespace pelican {


DataTypes::DataTypes()
    : _adapterFactory(0)
{
}

DataTypes::DataTypes(  const ConfigNode& config, AbstractAdapterFactory* factory )
    : _conf(config), _adapterFactory(factory)
{
    _adapterNames = _conf.getOptionHash("data", "type", "adapter");
}

/**
 *@details
 */
DataTypes::~DataTypes()
{
}

/**
 * @details
 * adds a data requirement. If the appropriate adapter
 * has not already been added with setAdater() then
 * this method will attemp to create a suitable one.
 */
void DataTypes::addData(const DataSpec& data)
{
    _dataRequirements.append(data);
    foreach( const QString& dataType, data.allData() ) {
        if( ! adapterAvailable( dataType ) ) {
            setAdapter(dataType, _createAdapter(dataType) );
        }
        else {
            _setAdapter( _dataRequirements.last(), dataType );
        }
    }
}

void DataTypes::addData(const QList<DataSpec>& data)
{
    foreach( const DataSpec& s, data) {
        addData(s);
    }
}

/**
 * @details
 * Sets a particular adapter for a particular type of data. All data added
 * with addData will be made consistent with the adpater
 */
void DataTypes::setAdapter(const QString& type, AbstractAdapter* adapter)
{
    _adapters[type] = adapter;
    // ensure each data requirement is of the correct type
    for(int i=0; i < _dataRequirements.size(); ++i )
    {
        DataSpec& req = _dataRequirements[i];
        _setAdapter( req, type );
    }
}

void DataTypes::_setAdapter( DataSpec& req, const QString& type )
{
    switch( _adapters[type]->type() )
    {
        case AbstractAdapter::Service :
            req.setServiceData( type );
            break;
        case AbstractAdapter::Stream :
            req.setStreamData( type );
            break;
        default:
            throw QString("unknown adapter type");
            break;
    }
}

AbstractStreamAdapter* DataTypes::streamAdapter(const QString& type) const
{
    Q_ASSERT( _adapters.contains(type) );
    Q_ASSERT( _adapters.value(type)->type() == AbstractAdapter::Stream );
    return static_cast<AbstractStreamAdapter*>(_adapters.value(type));
}

AbstractServiceAdapter* DataTypes::serviceAdapter(const QString& type) const
{
    Q_ASSERT( _adapters.contains(type) );
    Q_ASSERT( _adapters.value(type)->type() == AbstractAdapter::Service );
    return static_cast<AbstractServiceAdapter*>(_adapters.value(type));
}

AbstractAdapter::AdapterType_t DataTypes::type(const QString& dataName) const
{
    Q_ASSERT( _adapters.contains(dataName) );
    return _adapters.value(dataName)->type();
}

const QList<DataSpec>& DataTypes::dataSpec() const
{
    return _dataRequirements;
}

bool DataTypes::adapterAvailable( const QString& type ) const {
     return _adapters.contains(type);
}

AbstractAdapter* DataTypes::_createAdapter( const QString& dataType ) const {

    // Find the configuration information for adapters.
    return _adapterFactory->create( _adapterNames.value(dataType),
                _conf.getNamedOption("data","name","") );
}

} // namespace pelican
