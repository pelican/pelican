#ifndef IMAGERWRITERFITS_H_
#define IMAGERWRITERFITS_H_

#include "AbstractModule.h"
#include "data/ImageData.h"
#include "utility/ConfigNode.h"
#include <fitsio.h>
#include <QStringList>


/**
 * @file ImageWriterFits.h
 */

namespace pelican {

/**
 * @class ImageWriterFits
 *  
 * @brief
 * Module which will produce a brightness map given a matrix of visibility data.
 * 
 * @details
 */

class ImageWriterFits : public AbstractModule
{
    private:
        friend class ImageWriterFitsTest;

    public:
        /// Module constructor.
        ImageWriterFits(const ConfigNode& config);

        /// Module destructor.
        ~ImageWriterFits();

        /// Runs the module.
        void run(QHash<QString, DataBlob*>& data);

    public:
        /// Returns a reference to the image output directory.
        QString& directory() { return _directory; }

        /// Returns a reference to the filename.
        QString& fileName() { return _fileName; }

        /// Returns a reference to the filename prefix.
        QString& prefix() { return _prefix; }

        /// Returns a reference to the filename suffix.
        QString& suffix() { return _suffix; }

    private:
        /// Extract the configuration from the xml node setting default where required.
        void _getConfiguration(const ConfigNode& config);

        /// Opens the FITS image file for writing.
        void _open();

        /// Closes the FITS image file.
        void _close();

        /// Write a FITS image header
        void _writeHeader();

        /// Write a FITS image
        void _writeImage();

        /// Write a FITS image cube
        void _writeImageCube();

        /// Sets the FITS header date value from the system time.
        QString _getDate() const;

        /// Write a header key - string value
        void _writeKey(const QString& keyword, const QString& value,
                const QString& comment = QString());

        /// Write a header key - double value
        void _writeKey(const QString& keyword, const double& value,
                const QString& comment = QString());

        /// Write a header key - int value
        void _writeKey(const QString& keyword, const int& value,
                const QString& comment = QString());

        /// Write a header key - unsigned value
        void _writeKey(const QString& keyword, const unsigned& value,
                const QString& comment = QString());

    private:
        ImageData* _image;
        fitsfile* _fits;

        bool _overwrite;
        bool _cube;

        QString _directory;
        QString _fileName;
        QString _prefix;
        QString _suffix;

        QString _date;
        QString _origin;
        QString _dateObs;
        QString _telescope;
        QString _instrument;
        QString _observer;
        QString _object;
        QString _author;
        QString _reference;
        double _equinox;
};

} // namespace pelican

#endif // IMAGERFFT_H_
