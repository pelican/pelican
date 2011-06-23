#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <iostream>

#include "DataBlobFile.h"
#include "pelican/data/DataBlob.h"
#include "pelican/utility/ConfigNode.h"

namespace pelican {


/**
 * @details Constructs a DataBlobFile object.
 */
DataBlobFile::DataBlobFile(const ConfigNode& configNode)
    : AbstractOutputStream(configNode)
{
    // Get the filename from the configuration node, and open it for output.
    QList<QString> fileNames = configNode.getOptionList( "file", "name");
    foreach( const QString& filename, fileNames ) {
        addFile(filename);
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
void DataBlobFile::addFile(const QString& filename)
{
    verbose(QString("Creating file %1").arg(filename));
    QFile* file = new QFile(filename);
    if (file->open(QIODevice::WriteOnly)) {
        _devices.append(file);
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
        blob->serialise(*device);
    }
}

} // namespace pelican
