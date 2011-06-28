#ifndef DATABLOBFILE_H
#define DATABLOBFILE_H

#include <QtCore/QList>
#include <QtCore/QHash>
class QIODevice;
class QFile;

#include "DataBlobFileType.h"

#include "pelican/output/AbstractOutputStream.h"

/**
 * @file DataBlobFile.h
 */

namespace pelican {

/**
 * @class DataBlobFile
 * 
 * @brief
 *    write datablobs to a file
 * @details
 *    opens one or more files on a device and streams any
 *    DataBlob that supports the serialise() method to the file
 *    The file nay be of homogenous or mixed DataBlob types 
 *    (which incur a storage overhead).
 * @configuration
 * The default type of file is "homogenous". this must be set to heterogenous
 * if you intend to store different datablob types in the same file
 * <DataBlobFile>
 *     <file name="file1.output">
 *     <file name="duplicatefile.output" type="homogenous">
 *     <file name="hetroFormat.output" type="heterogenous">
 * <DataBlobFile>
 */
class DataBlobFile : public AbstractOutputStream
{

    public:
        /// DataBlobFile constructor.
        DataBlobFile(const ConfigNode& configNode);

        /// DataBlobFile destructor.
        virtual ~DataBlobFile();

        // Add a file for output to be saved.
        void addFile(const QString& filename, const DataBlobFileType::DataBlobFileType_t& type );

        // write the data
        virtual void sendStream(const QString& streamName,
                                const DataBlob* blob);


    private:
        QString _blobType;
        QList<QIODevice*> _devices;
        QHash<QIODevice*,bool> _hetero;
};

PELICAN_DECLARE(AbstractOutputStream, DataBlobFile )

} // namespace pelican

#endif // DATABLOBFILE_H 
