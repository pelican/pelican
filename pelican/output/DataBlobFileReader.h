#ifndef DATABLOBFILEREADER_H
#define DATABLOBFILEREADER_H
#include <QtCore/QString>

/**
 * @file DataBlobFileReader.h
 */

namespace pelican {
class DataBlobFileType;
class DataBlob;

/**
 * @class DataBlobFileReader
 *
 * @brief
 *    Read a file in the DataBlobFile format
 * @details
 *    This class provides a convenient way to read in DataBlobFile
 *    output streams. It can handle both heterogeneous and homogeneous
 *    file types
 *    The general procedure is:
 *    open() the file
 *    find out the next blob stored, with nextBlob()
 *    pass a DataBlob of this type to readData();
 */
class DataBlobFileReader
{
    public:
        /// DataBlobFileReader constructor.
        DataBlobFileReader();

        /// open the specified file for reading
        void open(const QString& filename);

        /// return the next DataBlob type in the stream
        QString nextBlob() const;

        /// read in data to the provided datablob
        void readData( DataBlob* blob );

        /// DataBlobFileReader destructor.
        ~DataBlobFileReader();

    private:
        DataBlobFileType* _file;
};

} // namespace pelican

#endif // DATABLOBFILEREADER_H
