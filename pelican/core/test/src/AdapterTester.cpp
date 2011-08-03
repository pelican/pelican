#include <QtCore/QFile>

#include "AdapterTester.h"
#include "pelican/core/AbstractStreamAdapter.h"
#include "pelican/core/AbstractServiceAdapter.h"


namespace pelican {
namespace test {


/**
 * @details Constructs a AdapterTester object.
 */
AdapterTester::AdapterTester(const QString& type, const QString& config)
   : _device(0)
{
    // Setup the XML configuration.
    Config::TreeAddress adapterBase;
    adapterBase << Config::NodeId("adapters", "");
    QString xml =
            "<adapters><" + type + ">"
          + config + "\n</" + type + ">\n</adapters>";
    _config.setXML(xml);
    _adapterFactory = new FactoryConfig<AbstractAdapter>(&_config, adapterBase );
    _object = _adapterFactory->create(type,"");
}

/**
 * @details Destroys the AdapterTester object.
 */
AdapterTester::~AdapterTester()
{
     delete _device;
     delete _adapterFactory; // _object gets deleted with factory
}

void AdapterTester::setDataFile(const QString& filename)
{
     if( _device )
        delete _device;
     _device = new QFile(filename);
     if( ! _device->open( QIODevice::ReadOnly) ) {
        throw( QString("cannot open file" + filename + " for reading") );
     }
     _bytes = _device->size();
}

void AdapterTester::setServiceData(QHash<QString, DataBlob*>& serviceData)
{
    _service = serviceData;
}

void AdapterTester::execute(DataBlob* blob )
{
     switch( _object->type() ) {
         case AbstractAdapter::Stream:
             ((AbstractStreamAdapter*)(_object))->config(blob, _bytes, _service);
             break;
         case AbstractAdapter::Service:
             ((AbstractServiceAdapter*)_object)->config(blob, _bytes);
             break;
         default:
             throw( QString("unknown adapter type :%1").arg( _object->type()) ) ;
     }
     _object->deserialise(_device);
}

} // test
} // namespace pelican
