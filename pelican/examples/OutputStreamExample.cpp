#include "OutputStreamExample.h"

#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <iostream>

#include "DataBlobExample.h"
#include "pelican/utility/ConfigNode.h"


OutputStreamExample::OutputStreamExample( const ConfigNode& configNode )
    : AbstractOutputStream( configNode )
{
    QString filename = configNode.getOption("file", "name");
    // initialise file
    if( filename != "" )
        addFile(filename);
}

OutputStreamExample::~OutputStreamExample()
{
    foreach( QIODevice* device, _devices )
    {
        delete device;
    }
}

void OutputStreamExample::addFile( const QString& filename )
{
    verbose(QString("creating file ") + filename);
    QFile* file = new QFile(filename);
    if( ! file->open( QIODevice::WriteOnly ) )
    {
        _devices.append( file );
    }
    else {
        std::cerr << "unable to open file for writing: " << filename.toStdString() << std::endl;
        delete file;
    }
}

void OutputStreamExample::sendStream(const QString& /*streamName*/, const DataBlob* dataBlob)
{
    // check its a datablob type we know what to do with
    if( dataBlob->type() != "DataBlobExample" )
        return;
    const DataBlobExample* data = static_cast<const DataBlobExample*>(dataBlob);
    
    // construct the comman seperated value string
    QString csv;
    if( data->size() > 0 )
    {
        csv = QString().setNum(data->data()[0]);
        for(unsigned int i=1; i < data->size(); ++i )
        {
            csv += "," + QString().setNum( data->data()[i] );
        }
        // output the string to each file
        foreach( QIODevice* device, _devices )
        {
            QTextStream out(device);
            out << csv << endl;
        }
    }
}

