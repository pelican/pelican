#include "modules/ImageWriterFits.h"
#include "utility/constants.h"
#include "utility/ConfigNode.h"
#include "data/ImageData.h"
#include <QDateTime>
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
    // Register which data blobs are needed by the module.
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
 *
 * @param[in] data  Hash of data blobs.
 */
void ImageWriterFits::run(QHash<QString, DataBlob*>& data)
{
    // grab image pointer to write
    _image = static_cast<ImageData*>(data["ImageData"]);
    if (!_image) throw QString("ImageWriterFits: Image data missing.");

    unsigned nL = _image->sizeL();
    unsigned nM = _image->sizeM();
    unsigned nChan = _image->nChannels();
    unsigned nPol = _image->nPolarisations();

    _open(_fileName, nL, nM, nChan, nPol, _overwrite);
    _writeHeader();
    for (unsigned c = 0; c < nChan; c++) {
        for (unsigned p = 0; p < nPol; p++) {
            real_t* image = _image->ptr(c, p);
            _writeImage(image, nL, nM, c, p);
        }
    }
    _close();
}


/**
 * @details
 * Parse and extract options from the configuration XML node, setting
 * defaults if necessary.
 * The following settings are read from the configuration if present:
 *
 * - General Options:
 *  - overwrite  = Determines if fits files should be overwritten if one of the same name exists
 *  - directory  = Output directory (NOT USED)
 *  - file       = The name of the output fits file (at present full path)
 *  - prefix     = File name prefix (NOT USED)
 *  - suffix     = File name suffix (NOT USED)
 *  - cube       = Option to control if image cubes or image slices are procuded (NOT USED)
 *
 * - Fits Header Options (These options get insertded into the fits header):
 *  - dateObs    = The date of the observation
 *  - origin     = The origin of the processed image
 *  - telesceope = The telescope use to make the observation
 *  - instrument = The instrument at the telescope
 *  - object     =
 *  - author     =
 *  - equinox    = The equinox (epoch) of the coordinate system used.
 *
 * @param[in]   config  Pelican XML configuration node.
 */
void ImageWriterFits::_getConfiguration(const ConfigNode &config)
{
    _overwrite = config.getOption("overwrite", "value", "true") == "true" ?
            true : false;
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
 * Opens a FITS image file for writing, creating an empty image of the specified
 * dimensions ready for writing into.
 *
 * @param[in] fileName  The filename of the fits file to write to (a fits file suffix is appended if missing)
 * @param[in] nL        The number of pixels in the l (x) direction
 * @param[in] nM        The number of pixels in the m (y) direction
 *
 */
void ImageWriterFits::_open(const QString& fileName, const unsigned nL,
        const unsigned nM, const unsigned nChan, const unsigned nPol,
        const bool overwrite)
{
    // Throw if the filename is empty
    if (fileName.isEmpty())
        throw QString("ImagerWriterFits: Output file name not set.");

    // Append fits file suffix if needed
    QFileInfo fileInfo(const_cast<QString&>(fileName));
    if (fileInfo.suffix().toLower() != "fits") {
        const_cast<QString&>(fileName) += ".fits";
    }
    _fileName = fileName;

    // Delete any existing file if overwrite mode is selected else throw if a file
    // already exits
    QFile file(fileName);
    if (file.exists()) {
        if (overwrite) file.remove();
        else throw QString("ImageWriterFits: File already exists.");
    }

    // Set axis dimensions to create the required FITS image.
    long nAxis = 4;
    long axisDims[] = {nL, nM, nPol, nChan};

    int err = 0; // CFITSIO error code.

    // Create the FITS file as an image of the specified dimensions.
    ffinit(&_fits, _fileName.toLatin1().data(), &err);
    ffrprt(stdout, err);
    if (err)
        throw QString("ImageWriterFits: Unable to open file for writing.");


    ffcrim(_fits, FLOAT_IMG, nAxis, axisDims, &err);
    if (err)
        throw QString("ImageWriterFits: Unable to create FITS image.");
}


/**
 * @details
 * This private method closes the FITS image file.
 */
void ImageWriterFits::_close()
{
    int err = 0;
    if (_fits != NULL) {
        ffclos(_fits, &err);
        if (err)
            throw QString("ImageWriterFits: Unable to close FITS file handle");
        _fits = NULL;
    }
}


/**
 * @details
 * This private method writes all FITS header keywords into the file.
 */
void ImageWriterFits::_writeHeader()
{
    // Throw if the CFITSIO file handle isn't open.
    if (_fits == NULL)
        throw QString("ImageWriterFits: Fits file not open.");

    // Throw if there is no image data.
    if (_image == NULL)
        throw QString("ImageWriterFits: Image data missing.");

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
    _writeKey("BUNIT", QString::fromStdString(_image->ampUnits()),
            "Units of flux");

    // Amplitude range (only valid if not an image cube)
    if (_image->nChannels() == 1 && _image->nPolarisations() == 1) {
        _writeKey("DATAMIN", _image->min(0, 0), "Minimum pixel value");
        _writeKey("DATAMAX", _image->max(0, 0), "Maximum pixel value");
    }
//    _writeComment("DATAMEAN = " + QString::number(_image->mean(0, 0)));

    // x axis keywords
    double rotaX = 0.0;
    _writeKey("CTYPE1", "RA---SIN");
    _writeKey("CUNIT1", "deg", "Axis unit (degrees)");
    _writeKey("CRPIX1", _image->refPixelL(), "Reference pixel");
    _writeKey("CRVAL1", _image->refCoordL(),
            "Coordinate value at reference point");
    _writeKey("CDELT1", _image->cellsizeL() * math::asec2deg,
            "Coordinate increment at reference point");
    _writeKey("CROTA1", rotaX);

    // y axis keywords
    double rotaY = 0.0;
    _writeKey("CTYPE2", "DEC--SIN");
    _writeKey("CUNIT2", "deg", "Axis unit (degrees)");
    _writeKey("CRPIX2", _image->refPixelM(), "Reference pixel");
    _writeKey("CRVAL2", _image->refCoordM(),
            "Coordinate value at reference point");
    _writeKey("CDELT2", _image->cellsizeM() * math::asec2deg,
            "Coordinate increment at reference point");
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
void ImageWriterFits::_writeImage(real_t* image, const unsigned nL,
        const unsigned nM, const unsigned chan, const unsigned pol)
{
    // Throw if the CFITSIO file handle isn't open.
    if (_fits == NULL) throw QString("ImageWriterFits: Fits file not open.");

    // Throw if there is no image data.
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
        throw QString("ImageWriterFits: Unrecognised data type.");
    }
}


/**
 * @details
 * Returns the system time and date for the DATE header keyword.
 * The system time is returned as UTC in the ISO 8601 extended
 * specification, taking the form YYYY-MM-DDTHH:MM:SS.
 */
QString ImageWriterFits::_getDate() const
{
    /* Get the system date */
    QDateTime utc = QDateTime::currentDateTime().toUTC();
    return utc.toString(Qt::ISODate);
}


/**
 * @details
 * Writes the given header \p keyword with string \p value and optional
 * \p comment.
 *
 * @param[in] keyword The keyword name tag.
 * @param[in] value   The keyword value.
 * @param[in] comment An optional comment.
 */
void ImageWriterFits::_writeKey(const QString& keyword, const QString& value,
        const QString& comment)
{
    int err = 0;
    ffpky(_fits, TSTRING, (char*)keyword.toLatin1().data(),
            (void*)value.toLatin1().data(), (char*)comment.toLatin1().data(),
            &err);
    if (err)
        throw QString("ImageWriterFits::_writeKey(): Error writing FITS key.");
}


/**
 * @details
 * Writes the given header \p keyword with floating-point double
 * \p value and optional \p comment.
 *
 * @param[in] keyword The keyword name tag.
 * @param[in] value   The keyword value.
 * @param[in] comment An optional comment.
 */
void ImageWriterFits::_writeKey(const QString& keyword, const double value,
        const QString& comment)
{
    int err = 0;
    ffpkyd(_fits, (char*)keyword.toLatin1().data(), value, 8,
            (char*)comment.toLatin1().data(), &err);
    if (err)
        throw QString("ImageWriterFits::_writeKey(): Error writing FITS key.");
}


/**
 * @details
 * Writes the given header \p keyword with integer \p value and optional
 * \p comment.
 *
 * @param[in] keyword The keyword name tag.
 * @param[in] value   The keyword value.
 * @param[in] comment An optional comment.
 */
void ImageWriterFits::_writeKey(const QString& keyword, const int value,
        const QString& comment)
{
    int err = 0;
    ffpky(_fits, TINT, (char*)keyword.toLatin1().data(), (void*)&value,
            (char*)comment.toLatin1().data(), &err);
    if (err)
        throw QString("ImageWriterFits::_writeKey(): Error writing FITS key.");
}


/**
 * @details
 * Writes the given header \p keyword with unsigned integer \p value and
 * optional \p comment.
 *
 * @param[in] keyword The keyword name tag.
 * @param[in] value   The keyword value.
 * @param[in] comment An optional comment.
 */
void ImageWriterFits::_writeKey(const QString& keyword, const unsigned value,
        const QString& comment)
{
    int err = 0;
    ffpky(_fits, TUINT, (char*)keyword.toLatin1().data(), (void*)&value,
            (char*)comment.toLatin1().data(), &err);
    if (err)
        throw QString("ImageWriterFits::_writeKey(): Error writing FITS key.");
}


/**
 * @details
 * Writes a history tag to the FITS header.
 *
 * @param[in] text The history line to write.
 */
void ImageWriterFits::_writeHistory(const QString& text)
{
    int err = 0;
    ffphis(_fits, (char*)text.toLatin1().data(), &err);
    if (err)
        throw QString("ImageWriterFits::_writeHistory(): Error writing FITS history.");
}


/**
 * @details
 * Writes a comment tag to the FITS header.
 *
 * @param[in] text The comment line to write.
 */
void ImageWriterFits::_writeComment(const QString& text)
{
    int err = 0;
    ffpcom(_fits, (char*)text.toLatin1().data(), &err);
    if (err)
        throw QString("ImageWriterFits::_writeComment(): Error writing FITS comment.");
}


/**
 * @details
 * Method to flip the image in the x-direction.
 *
 * @param[in] image Pointer to the start of the image.
 * @param[in] nL    Number of points along the x-axis.
 * @param[in] nM    Number of points along the y-axis.
 */
void ImageWriterFits::_flipXAxis(real_t* image, const unsigned nL,
        const unsigned nM)
{
    unsigned nPixels = nL * nM;
    std::vector<real_t> temp(nPixels);
    real_t* t = &temp[0];

    // Flip x-direction.
    for (unsigned i = 0, m = 0; m < nM; m++) {
        for (int l = (nL - 1); l >= 0; l--) {
            int index = m * nL + l;
            t[i] = image[index];
            i++;
        }
    }

    // Write back into the image.
    for (unsigned i = 0; i < nPixels; i++) {
        image[i] = t[i];
    }

}


/**
 * @details
 * Method to flip the image in the y-direction.
 *
 * @param[in] image Pointer to the start of the image.
 * @param[in] nL    Number of points along the x-axis.
 * @param[in] nM    Number of points along the y-axis.
 */
void ImageWriterFits::_flipYAxis(real_t* image, const unsigned nL,
                const unsigned nM)
{
    unsigned nPixels = nL * nM;
    std::vector<real_t> temp(nPixels);
    real_t* t = &temp[0];

    // Flip y-direction.
    for (int i = 0, m = nM - 1; m >= 0; m--) {
        for (unsigned l = 0; l < nL; l++) {
            unsigned index = m * nL + l;
            t[i] = image[index];
            i++;
        }
    }

    // Write back into the image.
    for (unsigned i = 0; i < nPixels; i++) {
        image[i] = t[i];
    }
}



/**
 * @details
 * Method to transpose the image (flip along the diagonal).
 *
 * @param[in] image Pointer to the start of the image.
 * @param[in] nL    Number of points along the x-axis.
 * @param[in] nM    Number of points along the y-axis.
 */
void ImageWriterFits::_transpose(real_t* image, const unsigned nL,
        const unsigned nM)
{
    unsigned nPixels = nL * nM;
    std::vector<real_t> temp(nPixels);
    real_t* t = &temp[0];

    // Transpose into a temporary.
    for (unsigned i = 0, m = 0; m < nM; m++) {
        for (unsigned l = 0; l < nL; l++) {
            unsigned index = l * nM + m;
            t[i] = image[index];
            i++;
        }
    }

    // Write back into the image.
    for (unsigned i = 0; i < nPixels; i++) {
        image[i] = t[i];
    }
}


} // namespace pelican
