#ifndef DATABLOBFILE_H
#define DATABLOBFILE_H

#include <QtCore/QList>
class QIODevice;

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
 * @configuration
 * <DataBlobFile>
 *     <file name="file1.output">
 *     <file name="duplicatefile.output">
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
        void addFile(const QString& filename);

        // write the data
        virtual void sendStream(const QString& streamName,
                                const DataBlob* blob);

    private:
        QList<QIODevice*> _devices;
};

PELICAN_DECLARE(AbstractOutputStream, DataBlobFile )

} // namespace pelican

#endif // DATABLOBFILE_H 
