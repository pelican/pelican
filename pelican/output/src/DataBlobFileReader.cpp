#include "DataBlobFileReader.h"
#include "DataBlobFileType.h"
#include <QtCore/QFile>


namespace pelican {


/**
 * @details Constructs a DataBlobFileReader object.
 */
DataBlobFileReader::DataBlobFileReader()
     : _file(0)
{
}

/**
 * @details Destroys the DataBlobFileReader object.
 */
DataBlobFileReader::~DataBlobFileReader()
{
    delete _file;
}

void DataBlobFileReader::open(const QString& filename)
{
    _file = DataBlobFileType::openExistingFile(filename, QIODevice::ReadOnly );
}

QString DataBlobFileReader::nextBlob() const {
    return _file->nextBlob();
}

void DataBlobFileReader::readData( DataBlob* blob ) {
    _file->readData(blob);
}

} // namespace pelican
