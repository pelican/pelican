#ifndef IMAGERWRITERFITS_H_
#define IMAGERWRITERFITS_H_

#include "pelican/modules/AbstractModule.h"
#include <QString>
#include <QHash>
#include <fitsio.h>
#include <vector>

/**
 * @file ImageWriterFits.h
 */

namespace pelican {

class ConfigNode;
class DataBlob;
class ImageData;

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
        void run(ImageData* image);

    public:
        /// Returns a reference to the image output directory.  // TODO use this
        QString& directory() { return _directory; }

        /// Returns a reference to the filename.
        QString& fileName() { return _fileName; }

        /// Returns a reference to the filename prefix.   // TODO use this
        QString& prefix() { return _prefix; }

        /// Returns a reference to the filename suffix.   // TODO use this
        QString& suffix() { return _suffix; }

    private:
        /// Extract the configuration from the XML node, setting defaults where required.
        void _getConfiguration(const ConfigNode& config);

        /// Opens the FITS image file for writing.
        void _open(const QString& fileName, const unsigned nL,
                const unsigned nM, const unsigned nChan, const unsigned nPol,
                const bool overwrite);

        /// Closes the FITS image file.
        void _close();

        /// Write a FITS image header.
        void _writeHeader(ImageData* image);

        /// Write a frequency table extension.
        void _writeFrequencyTable(const std::vector<unsigned>& channels);

        /// Write a FITS image.
        void _writeImage(real_t* image, const unsigned nL, const unsigned nM,
                const unsigned chan, const unsigned nPol, const unsigned pol);

        /// Returns the system date and time as UTC.
        QString _getDate() const;

        /// Write a header key - string value.
        void _writeKey(const QString& keyword, const QString& value,
                const QString& comment = QString());

        /// Write a header key - double value.
        void _writeKey(const QString& keyword, const double value,
                const QString& comment = QString());

        /// Write a header key - int value.
        void _writeKey(const QString& keyword, const int value,
                const QString& comment = QString());

        /// Write a header key - unsigned value.
        void _writeKey(const QString& keyword, const unsigned value,
                const QString& comment = QString());

        /// Write a header history line.
        void _writeHistory(const QString& text);

        /// Write a header comment line.
        void _writeComment(const QString& text);

    private:
        fitsfile* _fits;

        bool _overwrite;
        bool _cube;

        QString _directory;
        QString _fileName;
        QString _prefix;
        QString _suffix;

        QString _date;
        QString _origin;
        QString _telescope;
        QString _instrument;
        QString _observer;
        QString _object;
        QString _author;
        QString _reference;
};

} // namespace pelican

#endif // IMAGERFFT_H_
