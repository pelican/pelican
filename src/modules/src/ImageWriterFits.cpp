#include "modules/ImageWriterFits.h"
#include "utility/constants.h"
#include <ctime>
#include <QFile>


#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
ImageWriterFits::ImageWriterFits(const QDomElement& config)
    : AbstractModule(config)
{
    // Register which data blobs are needed by the module
    addLocalStreamData("ImageData");

    // Extract configuration from the xml configuration node.
    _getConfiguration(config);
}


/**
 * @details
 * Module destructor.
 */
ImageWriterFits::~ImageWriterFits()
{
}


/**
 * @details
 * Method called by the pipeline to create images of the visibility data.
 */
void ImageWriterFits::run(QHash<QString, DataBlob*>& data)
{
    // work out filename

    // if multiple file loop over them

    // grab image pointer to write

    // write the fits file
    _open();
    _writeHeader();
    _writeImage();
    //_writeImageCube();
    _close();
}


/**
 * @details
 * Parse and extract options from the configuration xml node.
 */
void ImageWriterFits::_getConfiguration(const QDomElement &config)
{
    _overwrite = getOption("overwrite", "value", "true") == "true" ? true : false;
    _directory = getOption("directory", "value", "");
    _fileName = getOption("file", "name", "pelican");
    _prefix = getOption("file", "prefix");
    _suffix = getOption("file", "suffix");
    _cube = getOption("cube", "value", "true") == "true" ? true : false;

    _dateObs = getOption("dateObs", "value");
    _origin = getOption("origin", "value");
    _telescope = getOption("telescope", "value");
    _instrument = getOption("instrument", "value");
    _object = getOption("object", "value");
    _author = getOption("author", "value");
    _equinox = getOption("equinox", "value", "2000").toDouble();
}


/**
 * @details
 * Opens a FITS image file for writing.
 */
bool ImageWriterFits::_open()
{
    // Check if the file to be opened for writing already exists.
    QFile file(_fileName);
    if (file.exists()) {
        if (_overwrite) file.remove();
        else return false;
    }

    int err = 0; // cfitsio error code
    long nAxis = 4; // Number of axes: l, m, polarisations, channels
    long axisDims[] = {
            _image->sizeL(),
            _image->sizeM(),
            _image->nPolarisations(),
            _image->nChannels()
    };

    fits_create_file(&_fits, _fileName.toLatin1().data(), &err);
    if (err) throw QString("ImageWriterFits: Unable to open file for writing");

    fits_create_img(_fits, FLOAT_IMG, nAxis, axisDims, &err);
    if (err) throw QString("ImageWriterFits: Unable to create FITS image");

    return true;
}


/**
 * @details
 */
void ImageWriterFits::_close()
{
    int err = 0;
    fits_close_file(_fits, &err);
}



/**
 * @details
 */
void ImageWriterFits::_writeHeader()
{
    // Write descriptive keys
    _writeKey("DATE", _getDate());
    _writeKey("ORIGIN", _origin);
    _writeKey("DATE-OBS", _dateObs);
    _writeKey("TELESCOP", _telescope);
    _writeKey("INSTRUME", _instrument);
    _writeKey("OBSERVER", _observer);
    _writeKey("OBJECT", _object);
    _writeKey("AUTHOR", _author);
    _writeKey("REFERENC", _reference);

    // Brightness scaling and coordinate
    double bscale = 0.0;
    double bzero = 0.0;
    double ra = 0.0;
    double dec = 0.0;
    _writeKey("BSCALE", bscale);
    _writeKey("BZERO", bzero);
    _writeKey("BUNIT", "JA/BEAM", "Units of flux");
    _writeKey("EQUINOX", _equinox);
    _writeKey("OBSRA", ra);
    _writeKey("OBSDEC", dec);

    // Amplitude range
    _image->findAmpRange();
    _writeKey("DATAMIN", _image->min(), "Minimum pixel value");
    _writeKey("DATAMAX", _image->max(), "Maximum pixel value");

    // x axis keywords
    double rotaX = 0.0;
    _writeKey("CTYPE1", "L----SIN");
    _writeKey("CRVAL1", _image->refCoordL(), "Reference pixel value");
    _writeKey("CDELT1", _image->cellsizeL() * math::asec2deg, "Degrees per pixel");
    _writeKey("CRPIX1", _image->refPixelL(), "Reference pixel");
    _writeKey("CROTA1", rotaX);

    // y axis keywords
    double rotaY = 0.0;
    _writeKey("CTYPE2", "M----SIN");
    _writeKey("CRVAL2", _image->refCoordM(), "Reference pixel value");
    _writeKey("CDELT2", _image->cellsizeM() * math::asec2deg, "Degrees per pixel");
    _writeKey("CRPIX2", _image->refPixelM(), "Reference pixel");
    _writeKey("CROTA2", rotaY);

    // polarisation axis keywords
    _writeKey("CTYPE3", "POL", "XX / YY");
    _writeKey("CRVAL3", 0.0);
    _writeKey("CDELT3", 0.0);
    _writeKey("CRPIX3", 0.0);
    _writeKey("CROTA3", 0.0);

    // channel axis keywords
    _writeKey("CTYPE4", "FREQ");
    _writeKey("CRVAL4", 0.0);
    _writeKey("CDELT4", 0.0);
    _writeKey("CRPIX4", 0.0);
    _writeKey("CROTA4", 0.0);
}


/**
 * @details
 */
void ImageWriterFits::_writeImage()
{
    int err = 0;
    real_t* image = _image->ptr();
    long nPixels = _image->nPixels();
    fits_write_img(_fits, TFLOAT, 1, nPixels, &image[0], &err);
}


/**
 * @details
 */
void ImageWriterFits::_writeImageCube()
{

}


/**
 * @details
 * Sets the DATE header keyword value from the system time.
 */
QString ImageWriterFits::_getDate() const
{
    std::time_t t = time(NULL);
    struct tm* timeinfo = localtime(&t);
    return QString(asctime(timeinfo));
}


/**
 * @details
 */
void ImageWriterFits::_writeKey(const QString& keyword, const QString& value,
        const QString& comment)
{
    int err = 0;
    char* k = keyword.toLatin1().data();
    char* v = value.toLatin1().data();
    char* c = comment.toLatin1().data();
    fits_write_key(_fits, TSTRING, k, v, c, &err);
}


/**
 * @details
 */
void ImageWriterFits::_writeKey(const QString& keyword, const double& value,
        const QString& comment)
{
    int err = 0;
    char* k = keyword.toLatin1().data();
    double v = value;
    char* c = comment.toLatin1().data();
    fits_write_key(_fits, TDOUBLE, k, &v, c, &err);
}



/**
 * @details
 */
void ImageWriterFits::_writeKey(const QString& keyword, const long& value,
        const QString& comment)
{
    int err = 0;
    char* k = keyword.toLatin1().data();
    long v = value;
    char* c = comment.toLatin1().data();
    fits_write_key(_fits, TDOUBLE, k, &v, c, &err);
}

} // namespace pelican
