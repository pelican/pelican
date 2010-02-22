#include "modules/ImageWriterFits.h"
#include "utility/constants.h"
#include "utility/ConfigNode.h"
#include "data/ImageData.h"
#include <ctime>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <typeinfo>
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
    _image = static_cast<ImageData*>(data["ImageData"]);
    if (!_image) throw QString("ImageWriterFits: Image data missing.");

    // write the fits file
    _open();
    _writeHeader();

    unsigned chan = 0;
    unsigned pol = 0;
    real_t* image = _image->ptr(pol, chan);
    unsigned nL = _image->sizeL();
    unsigned nM = _image->sizeM();

    _writeImage(image, nL, nM, chan, pol);
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
    // Throw if the filename is empty
    if (_fileName.isEmpty()) throw QString("ImagerWriterFits: Output file name not set");

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
    double bscale = 1.0;
    double bzero = 0.0;
    _writeKey("BSCALE", bscale);
    _writeKey("BZERO", bzero);
    _writeKey("EQUINOX", _equinox);
    _writeKey("BUNIT", QString::fromStdString(_image->ampUnits()), "Units of flux");
    double ra = 0;
    double dec = 90;
    _writeKey("OBSRA", ra);
    _writeKey("OBSDEC", dec);

    // Amplitude range
    _image->findAmpRange(0,0);
    _writeKey("DATAMIN", _image->min(0, 0), "Minimum pixel value");
    _writeKey("DATAMAX", _image->max(0, 0), "Maximum pixel value");

    // x axis keywords
    double rotaX = 0.0;
    _writeKey("CTYPE1", "RA---SIN");
//    _writeKey("CRVAL1", _image->refCoordL(), "Coordinate value at reference point");
//    _writeKey("CDELT1", _image->cellsizeL() * math::asec2deg, "Coordinate increment at reference point (deg)");
//    _writeKey("CRPIX1", _image->refPixelL(), "Reference pixel");
    _writeKey("CRVAL1", 0, "Coordinate value at reference point");
    _writeKey("CDELT1", -(2.0 / _image->sizeL()) * math::rad2deg, "Coordinate increment at reference point (deg)");
    _writeKey("CRPIX1", _image->sizeL()/2, "Reference pixel (image centre)");

    _writeKey("CUNIT1", "deg", "Axis unit (degrees)");
    _writeKey("CROTA1", rotaX);

    // y axis keywords
    double rotaY = 0.0;
    _writeKey("CTYPE2", "DEC--SIN");
//    _writeKey("CRVAL2", _image->refCoordM(), "Coordinate value at reference point");
//    _writeKey("CDELT2", _image->cellsizeM() * math::asec2deg, "Coordinate increment at reference point (deg)");
//    _writeKey("CRPIX2", _image->refPixelM(), "Reference pixel");
    _writeKey("CRVAL2", 90, "Coordinate value at reference point");
    _writeKey("CDELT2", (2.0 / _image->sizeM()) * math::rad2deg, "Coordinate increment at reference point (deg)");
    _writeKey("CRPIX2", 1 + _image->sizeM()/2, "Reference pixel (image centre)");

    _writeKey("CUNIT2", "deg", "Axis unit (degrees)");
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
    _writeHistory("- PELICAN: Pipeline for Extensible Lightweight Imaging and CAlibratioN");
//    _writeHistory("- SEE: www.pelican.oerc.ox.ac.uk");
}


/**
 * @details
 */
void ImageWriterFits::_writeImage(real_t* image, unsigned& nL, unsigned &nM,
        unsigned& chan, unsigned& pol)
{
    // Throw if the cfitsio FITS file handle isn't open
    if (_fits == NULL) throw QString("ImageWriterFits: Fits file not open.");
    // Throw if there is no image data
    if (_image == NULL) throw QString("ImageWriterFits: Image data missing.");


    // Write out the image.
    long nPixels = nL * nM;
    unsigned nPixPerChan = _image->nPolarisations() * nPixels;

    long firstElement = chan * nPixPerChan + pol * nPixels + 1;

    int err = 0;
    if (typeid(real_t) == typeid(double)) {
        ffppr(_fits, TDOUBLE, firstElement, nPixels, image, &err);
    }
    else if (typeid(real_t) == typeid(float)) {
        ffppr(_fits, TFLOAT, firstElement, nPixels, image, &err);
    }
    else {
        throw QString("ImageWriterFits: unrecognised data type size");
    }
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



/// flip the image in the x direction
void ImageWriterFits::_flipXAxis(real_t* image, unsigned& nL, unsigned& nM)
{
    unsigned nPixels = nL * nM;
    std::vector<real_t> temp(nPixels);
    real_t* t = &temp[0];

    // Flip x direction
    for (unsigned i = 0, m = 0; m < nM; m++) {
        for (int l = (nL - 1); l >= 0; l--) {
            int index = m * nL + l;
            t[i] = image[index];
            i++;
        }
    }

    // write back into the image
    for (unsigned i = 0; i < nPixels; i++) {
        image[i] = t[i];
    }

}


/// flip the image in the y direction
void ImageWriterFits::_flipYAxis(real_t* image, unsigned& nL, unsigned& nM)
{
    unsigned nPixels = nL * nM;
    std::vector<real_t> temp(nPixels);
    real_t* t = &temp[0];

    // Flip y direction
    for (int i = 0, m = nM - 1; m >= 0; m--) {
        for (unsigned l = 0; l < nL; l++) {
            unsigned index = m * nL + l;
            t[i] = image[index];
            i++;
        }
    }

    // Write back into the image
    for (unsigned i = 0; i < nPixels; i++) {
        image[i] = t[i];
    }
}


/// Transpose the image
void ImageWriterFits::_transpose(real_t* image, unsigned& nL, unsigned& nM)
{
    unsigned nPixels = nL * nM;
    std::vector<real_t> temp(nPixels);
    real_t* t = &temp[0];

    // transpose into a temp
    for (unsigned i = 0, m = 0; m < nM; m++) {
        for (unsigned l = 0; l < nL; l++) {
            unsigned index = l * nM + m;
            t[i] = image[index];
            i++;
        }
    }

    // write back into the image
    for (unsigned i = 0; i < nPixels; i++) {
        image[i] = t[i];
    }
}


} // namespace pelican
