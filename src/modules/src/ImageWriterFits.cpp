#include "modules/ImageWriterFits.h"
#include "utility/constants.h"
#include "utility/ConfigNode.h"
#include "data/ImageData.h"
#include <ctime>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
ImageWriterFits::ImageWriterFits(const ConfigNode& config)
    : AbstractModule(config)
{
    // Register which data blobs are needed by the module
    addGeneratedData("ImageData");

    // Extract configuration from the XML configuration node.
    _getConfiguration(config);

    _image = NULL;
    _fits = NULL;
}


/**
 * @details
 * Module destructor.
 */
ImageWriterFits::~ImageWriterFits()
{
    _close();
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
void ImageWriterFits::_getConfiguration(const ConfigNode &config)
{
    _overwrite = config.getOption("overwrite", "value", "true") == "true" ? true : false;
    _directory = config.getOption("directory", "value", QDir::currentPath());
    _fileName = config.getOption("file", "name", "pelican.fits");
    _prefix = config.getOption("file", "prefix");
    _suffix = config.getOption("file", "suffix");
    _cube = config.getOption("cube", "value", "true") == "true" ? true : false;

    _dateObs = config.getOption("dateObs", "value");
    _origin = config.getOption("origin", "value");
    _telescope = config.getOption("telescope", "value");
    _instrument = config.getOption("instrument", "value");
    _object = config.getOption("object", "value");
    _author = config.getOption("author", "value");
    _equinox = config.getOption("equinox", "value", "2000").toDouble();
}


/**
 * @details
 * Opens a FITS image file for writing.
 */
void ImageWriterFits::_open()
{
    // Throw if there is no image data.
    if (_image == NULL) throw QString("ImageWriterFits: Image data missing.");

    // Throw if the filename is empty
    if (_fileName.isEmpty()) throw QString("ImagerWriterFits: Output file empty");

    // Append fits file suffix if neede
    QFileInfo fileInfo(_fileName);
    if (fileInfo.suffix().toLower() != "fits") _fileName += ".fits";

    // Delete any existing file if overwrite mode is selected else throw if a file
    // already exits
    QFile file(_fileName);
    if (file.exists()) {
        if (_overwrite) file.remove();
        else throw QString("ImageWriterFits: File already exists");
    }

    // Set axis dimensions require to create a fits image
    long nAxis = 4;
    long axisDims[] = {
            _image->sizeL(),
            _image->sizeM(),
            _image->nPolarisations(),
            _image->nChannels()
    };

    int err = 0; // cfitsio error code

    // Create the fits file as an image of the specified dimensions
    fits_create_file(&_fits, _fileName.toLatin1().data(), &err);
    if (err) throw QString("ImageWriterFits: Unable to open file for writing");
    fits_create_img(_fits, FLOAT_IMG, nAxis, axisDims, &err);
    if (err) throw QString("ImageWriterFits: Unable to create FITS image");
}


/**
 * @details
 */
void ImageWriterFits::_close()
{
    int err = 0;
    if (_fits != NULL) fits_close_file(_fits, &err);
}



/**
 * @details
 */
void ImageWriterFits::_writeHeader()
{
    // Throw if the cfitsio FITS file handle isn't open
    if (_fits == NULL) throw QString("ImageWriterFits: Fits file not open.");
    // Throw if there is no image data
    if (_image == NULL) throw QString("ImageWriterFits: Image data missing.");

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
    _writeKey("BUNIT", "JY/BEAM", "Units of flux");
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

    _writeHistory("This image was created using PELICAN.");
    _writeHistory("- PELICAN: Pipeline for Extensible Imaging and CalibrAtion");
//    _writeHistory("- SEE: www.pelican.oerc.ox.ac.uk");
}


/**
 * @details
 */
void ImageWriterFits::_writeImage()
{
    // Throw if the cfitsio FITS file handle isn't open
    if (_fits == NULL) throw QString("ImageWriterFits: Fits file not open.");
    // Throw if there is no image data
    if (_image == NULL) throw QString("ImageWriterFits: Image data missing.");

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
    ffpky(_fits, TSTRING, (char*)keyword.toLatin1().data(),
            (void*)value.toLatin1().data(), (char*)comment.toLatin1().data(),
            &err);
}


/**
 * @details
 */
void ImageWriterFits::_writeKey(const QString& keyword, const double& value,
        const QString& comment)
{
    int err = 0;
    ffpky(_fits, TDOUBLE, (char*)keyword.toLatin1().data(), (void*)&value,
            (char*)comment.toLatin1().data(), &err);
}


/**
 * @details
 */
void ImageWriterFits::_writeKey(const QString& keyword, const int& value,
        const QString& comment)
{
    int err = 0;
    ffpky(_fits, TINT, (char*)keyword.toLatin1().data(), (void*)&value,
            (char*)comment.toLatin1().data(), &err);
}

/**
 * @details
 */
void ImageWriterFits::_writeKey(const QString& keyword, const unsigned& value,
        const QString& comment)
{
    int err = 0;
    ffpky(_fits, TUINT, (char*)keyword.toLatin1().data(), (void*)&value,
            (char*)comment.toLatin1().data(), &err);
}


void ImageWriterFits::_writeHistory(const QString& text)
{
    int err = 0;
    ffphis(_fits, (char*)text.toLatin1().data(), &err);
}


} // namespace pelican
