#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <iostream>

#include "DataBlobFile.h"
#include "DataBlobFileType.h"
#include "pelican/data/DataBlob.h"
#include "pelican/utility/ConfigNode.h"

namespace pelican {


/**
 * @details Constructs a DataBlobFile object.
 */
DataBlobFile::DataBlobFile(const ConfigNode& configNode)
    : AbstractOutputStream(configNode), _blobType("")
{
    // Get the filename from the configuration node, and open it for output.
    const QDomElement& node = configNode.getDomElement();
    QDomNodeList list = node.elementsByTagName("file");
    for (int i = 0; i < list.size(); ++i) {
        QDomElement element = list.at(i).toElement();
        if (element.hasAttribute("name")) {
            QString type;
            if (element.hasAttribute("type")) {
                type = element.attribute("type");
            }
            else { type = "homogeneous"; }
            if( type == "homogeneous" ) {
                addFile( element.attribute("name"), DataBlobFileType::Homogeneous );
            }
            else if( type == "heterogeneous" ) {
                addFile( element.attribute("name"), DataBlobFileType::Heterogeneous );
            }
            else {
                throw( QString("DataBlobFile: unknown file type specified for file %1:")
                        .arg(element.attribute("name")) + type );
            }
        }
    }
}

/**
 * @details Destroys the DataBlobFile object.
 */
DataBlobFile::~DataBlobFile()
{
    foreach (QIODevice* device, _devices) {
        delete device;
    }
}

// Adds a file to the output stream and opens it for writing.
void DataBlobFile::addFile(const QString& filename, const DataBlobFileType::DataBlobFileType_t& type )
{
    verbose(QString("Creating file %1").arg(filename));
    QFile* file = new QFile(filename);
    if (file->open(QIODevice::WriteOnly)) {
        QDataStream out(file);
        out.setVersion(QDataStream::Qt_4_0);
        _devices.append(file);
        _hetero.insert(file, ((type==DataBlobFileType::Heterogeneous)?true:false) );
        out << QString(DATABLOBFILE_MAGIC);
        out << DATABLOBFILE_VERSION;
        out << QSysInfo::ByteOrder;
        out << type;
    }
    else {
        std::cerr << "Cannot open file for writing: "
                  << filename.toStdString() << std::endl;
        delete file;
    }
}

// Sends the data blob to the output stream.
void DataBlobFile::sendStream(const QString& /*streamName*/,
        const DataBlob* blob)
{
    // Output the string to each file
    foreach (QIODevice* device, _devices) {
        const QString& type=blob->type();
        if( _hetero[device] ) {
            // mark the blob type for heterogeneous blob files
            DataBlobFileType::writeType( type, device);
        }
        else if( type !=  _blobType ) {
             if( _blobType == "" ) {
                 DataBlobFileType::writeType( type, device);
                 _blobType=type;
             }
             else {
                verbose( QString("Attempt to write a DataBlob of type " + type 
                             + " to homogenous file of type " + _blobType ));
                continue;
             }
        }
        blob->serialise(*device);
    }
}


} // namespace pelican
