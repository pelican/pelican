#ifndef DATABLOBFILETYPE_H
#define DATABLOBFILETYPE_H
#include <QtCore/QString>
#include <QtCore/QIODevice>
#include <QtCore/QSysInfo>
class QFile;

// type and version identifier for file header
#define DATABLOBFILE_MAGIC "DBlob"
#define DATABLOBFILE_VERSION 1
/**
 * @file DataBlobFileType.h
 */

namespace pelican {
class DataBlob;

/**
 * @class DataBlobFileType
 *
 * @brief
 *    Common data types for the DataBlobFile and DataBlobFileReader
 * @details
 *
 */
class DataBlobFileType
{
    public:
        typedef enum { Homogeneous, Heterogeneous } DataBlobFileType_t ;

    public:
        /// DataBlobFileType constructor.
        DataBlobFileType(QFile* file, QSysInfo::Endian endian);

        /// DataBlobFileType destructor.
        virtual ~DataBlobFileType();

        /// return the next datablob
        virtual QString nextBlob() const;

        /// read in data from the Blob
        virtual void readData(DataBlob* blob);

        static DataBlobFileType* openExistingFile(const QString& filename, QIODevice::OpenModeFlag);
        static void writeType(const QString& type, QIODevice* device);

    protected:
        void _readType();

    protected:
        QString _blobType;
        QFile* _file;
        QSysInfo::Endian _endian;

    private:
};

class DataBlobFileHeterogeneous : public DataBlobFileType {
    public:
        DataBlobFileHeterogeneous(QFile*, QSysInfo::Endian endian);
        void readData(DataBlob* blob);
};

class DataBlobFileHomogeneous: public DataBlobFileType {
    public:
        DataBlobFileHomogeneous(QFile*, QSysInfo::Endian endian);
};

} // namespace pelican

#endif // DATABLOBFILETYPE_H
