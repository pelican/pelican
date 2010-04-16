#include "pelican/modules/ImageWriterFits.h"

#include "pelican/utility/constants.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/data/ImageData.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <typeinfo>
#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

PELICAN_DECLARE_MODULE(ImageWriterFits)

/**
 * @details
 * Module constructor.
 */
ImageWriterFits::ImageWriterFits(const ConfigNode& config)
    : AbstractModule(config)
{
    // Extract configuration from the XML configuration node.
    _getConfiguration(config);

    // Initialise FITS file handle.
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
 * Method called by the pipeline to create write image data blobs to FITS format.
 *
 * @param[in] data  Hash of data blobs.
 */
void ImageWriterFits::run(ImageData* image)
{
    if (!image)
        throw QString("ImageWriterFits: Image data missing.");

    // Image dimensions.
    unsigned nL = image->sizeL();
    unsigned nM = image->sizeM();
    unsigned nChan = image->nChannels();
    unsigned nPol = image->nPolarisations();

    // Open the FITS file handle.
    _open(_fileName, nL, nM, nChan, nPol, _overwrite);

    // Write the FITS header.
    _writeHeader(image);

    // Loop over channels and polarisations in the image data writing
    // planes of the FITS image.
    for (unsigned c = 0; c < nChan; c++) {
        for (unsigned p = 0; p < nPol; p++) {
            real_t* data = image->ptr(c, p);
            _writeImage(data, nL, nM, c, nPol, p);
        }
    }

    // Write a frequency table (needed for image cubes)
    if (nChan > 1) {
        _writeFrequencyTable(image->channels());
    }

    // Close the FITS file handle.
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
 *  - cube       = Option to control if image cubes or image slices are produced (NOT USED)
 *
 * - Fits Header Options (These options get inserted into the fits header):
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

    _origin = config.getOption("origin", "value");
    _telescope = config.getOption("telescope", "value");
    _instrument = config.getOption("instrument", "value");
    _object = config.getOption("object", "value");
    _author = config.getOption("author", "value");
}


/**
 * @details
 * Opens a FITS image file for writing, creating an empty image of the specified
 * dimensions ready for writing.
 *
 * @param[in] fileName  The filename of the fits file to write to (a fits file suffix is appended if missing)
 * @param[in] nL        The number of pixels in the l (x) direction
 * @param[in] nM        The number of pixels in the m (y) direction
 * @param[in] nChan     The number of channels in the image cube.
 * @param[in] nPol      The number of polarisations in the image cube.
 * @param[in] overwrite Option flag to overwrite images files if they already
 *                      exist.
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
void ImageWriterFits::_writeHeader(ImageData* image)
{
    // Throw if the CFITSIO file handle isn't open.
    if (_fits == NULL)
        throw QString("ImageWriterFits: Fits file not open.");

    // Throw if there is no image data.
    if (image == NULL)
        throw QString("ImageWriterFits: Image data missing.");

    // Write descriptive keys.
    _writeKey("DATE", _getDate());
    _writeKey("ORIGIN", _origin);
    //_writeKey("DATE-OBS", "----"); //TODO: Set DATE-OBS from datablob
    _writeKey("TELESCOP", _telescope);
    _writeKey("INSTRUME", _instrument);
    _writeKey("OBSERVER", _observer);
    _writeKey("OBJECT", _object);
    _writeKey("AUTHOR", _author);
    _writeKey("REFERENC", _reference);

    // Brightness scaling and coordinate.
    double bscale = 1.0;
    double bzero = 0.0;
    _writeKey("BSCALE", bscale);
    _writeKey("BZERO", bzero);
    if (image->coordType() == ImageData::COORD_J2000)
        _writeKey("EQUINOX", "2000");
    _writeKey("BUNIT", image->ampUnits(), "Units of flux");

    // Amplitude range (only valid if not an image cube).
    if (image->nChannels() == 1 && image->nPolarisations() == 1) {
        _writeKey("DATAMIN", image->min(0, 0), "Minimum pixel value");
        _writeKey("DATAMAX", image->max(0, 0), "Maximum pixel value");
    }

    // x (l) axis keywords.
    _writeKey("CTYPE1", "RA---SIN");
    _writeKey("CUNIT1", "deg", "Axis unit (degrees)");
    _writeKey("CRVAL1", image->refCoordL(),
            "Coordinate value at reference point");
    _writeKey("CRPIX1", image->refPixelL(), "Reference pixel");
    _writeKey("CDELT1", image->cellsizeL() * math::asec2deg,
            "Coordinate increment at reference point");
//    _writeKey("CROTA1", 0.0);

    // y (m) axis keywords.
    _writeKey("CTYPE2", "DEC--SIN");
    _writeKey("CUNIT2", "deg", "Axis unit (degrees)");
    _writeKey("CRVAL2", image->refCoordM(),
            "Coordinate value at reference point");
    _writeKey("CRPIX2", image->refPixelM(), "Reference pixel");
    _writeKey("CDELT2", image->cellsizeM() * math::asec2deg,
            "Coordinate increment at reference point");
//    _writeKey("CROTA2", 0.0);

    // Polarisation axis keywords.
    // TODO: Set polarisation FITS axes
    _writeKey("CTYPE3", "POL", "XX / YY");
    int polType = (image->polarisation() == POL_Y) ? 1 : 0;
    _writeKey("CRVAL3", polType, "0.0 if X or X and Y, 1.0 if Y only");
    int polDelta = (image->polarisation() == POL_BOTH) ? 1 : 0;
    _writeKey("CDELT3", polDelta, "0.0 or 1.0 (if there is X and Y)");
    _writeKey("CRPIX3", 0.0);
    // _writeKey("CROTA3", 0.0);

    // Channel axis keywords.
    // TODO: Set channel FITS axes (including image cube channel table)
    _writeKey("CTYPE4", "FREQ");
    _writeKey("CRVAL4", image->refFreq());
    _writeKey("CDELT4", image->deltaFreq());
    _writeKey("CRPIX4", image->refChannel());
//    _writeKey("CROTA4", 0.0);

    _writeHistory("This image was created using PELICAN.");
    _writeHistory("- PELICAN: Pipeline for Extensible Lightweight Imaging and CAlibratioN");
}


/**
 * @details
 * Write a FITS frequency table extension holding the list of channels in the
 * image data cube.
 *
 * @note
 * See http://goo.gl/Es3S for reference on fits tables with cfitsio.
 */
void ImageWriterFits::_writeFrequencyTable(const std::vector<unsigned>& channels)
{
    // Throw if the CFITSIO file handle isn't open.
    if (_fits == NULL)
        throw QString("ImageWriterFits: Fits file handle not open.");

    // The naxis2 parameter gives the initial number of rows to be created in
    // the table, and should normally be set = 0. CFITSIO will automatically
    // increase the size of the table as additional rows are written.
    long initRows = 0;
    char* extname = NULL;
    char** tunit = NULL;
    int status = 0;
    int tfields = 1;
    char *ttype[] = { (char*)"channels" };
    // 4 wide so channel indices have to be in the range 0 - 9999
    char *tform[] = { (char*)"I4" };

    ffcrtb(_fits, ASCII_TBL, initRows, tfields, ttype, tform, tunit, extname,
            &status);
    if (status)
            throw QString("ImageWriterFits: Unable to create frequency table.");

    long nRows = channels.size();
    unsigned* c = const_cast<unsigned*>(&channels[0]);
    ffpcl(_fits, TUINT, 1, 1, 1, nRows, c, &status);

    if (status)
        throw QString("ImageWriterFits: Unable to create frequency table.");
}


/**
 * @details
 * Write image data to the FITS primary data array.
 * See: http://heasarc.nasa.gov/docs/software/fitsio/c/c_user/node75.html#ffppr
 *
 * @param[in] image     Image amplitude data matrix.
 * @param[in] nL        Number of pixels in the l (x) direction.
 * @param[in] nM        Number of pixels in the m (y) direction.
 * @param[in] chan      Image channel index.
 * @param[in] nPol      Total number of polarisations in the image data.
 * @param[in] pol       Image polarisation index.
 */
void ImageWriterFits::_writeImage(real_t* image, const unsigned nL,
        const unsigned nM, const unsigned chan, const unsigned nPol,
        const unsigned pol)
{
    // Throw if the CFITSIO file handle isn't open.
    if (_fits == NULL)
        throw QString("ImageWriterFits: Fits file handle not open.");

    // Throw if there is no image data.
    if (image == NULL)
        throw QString("ImageWriterFits: Image data array missing.");

    // Write out the image.
    long nPixels = nL * nM;
    unsigned nPixPerChan = nPol * nPixels;

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
        throw QString("ImageWriterFits::_writeHistory(): Error writing history.");
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
        throw QString("ImageWriterFits::_writeComment(): Error writing comment.");
}


} // namespace pelican
