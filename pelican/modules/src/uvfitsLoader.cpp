#include "pelican/modules/uvfitsLoader.h"
#include <QString>


namespace pelican {

PELICAN_DECLARE_MODULE(uvfitsLoader)

uvfitsLoader::uvfitsLoader(const ConfigNode& config)
{
    // TODO initialise variables here!

    _getConfiguration(config);
}

uvfitsLoader::~uvfitsLoader() {
    // TODO Auto-generated destructor stub
}


/**
 * @details
 * Loads the visibility fits file into the data blob.
 */
void uvfitsLoader::run(VisibilityData* rawVis)
{
    _readHeader();
    _readData();
}


/// Read the FITS header.
void uvfitsLoader::_readHeader()
{
    // Check if the file is random groups UV FITS
    if (_readKeyStr("GROUPS") != "T")
        throw err("File is not random groups UVFITS format.");

    if (_readKeyStr("BITPIX") != "-32")
        throw err("UV data is not the (assumed) single precision floating point format");

    // Read the telescope and set the number of antenna
    QString telescope = _readKeyStr("TELESCOP");

    if (telescope == "GMRT") {
        _nAntennas = 30;
    }
    else if (telescope == "VLA") {
        _nAntennas = 27;
    }
    else {
        throw err("Unknown telescope. Unable to set number of antennas.");
    }

    // Read the number of random groups.
    _nGroupsTotal = _readKeyLng("GCOUNT");

    // Read the number of data group axes.
    _nAxes = _readKeyLng("NAXIS") - 1;

    _ctype.resize(_naxis);
    _crval.resize(_naxis);
    _cdelt.resize(_naxis);
    _crpix.resize(_naxis);
    _crota.resize(_naxis);
    _readKeysStr("CTYPE", 2, _nAxis + 1, _ctype);
    _readKeysLng("NAXIS", 2, _nAxis + 1, _naxis);
    _readKeysDbl("CRVAL", 2, _nAxis + 1, _crval);
    _readKeysDbl("CDELT", 2, _nAxis + 1, _cdelt);
    _readKeysDbl("CRPIX", 2, _nAxis + 1, _crpix);
    _readKeysDbl("CROTA", 2, _nAxis + 1, _crota);
    for (unsigned i = 0; i < nAxis; i++) {
        _axes[_ctype[i]] = i;
    }

    // Read the axis parameters
    _nParametersPerGroup = _readKeyLng("PCOUNT");
    _ptype.resize(_nParametersPerGroup);
    _pzero.resize(_nParametersPerGroup);
    _pscal.resize(_nParametersPerGroup);
    _readKeysStr("PTYPE", 1, _nParametersPerGroup, _ptype);
    _readKeysDbl("PZERO", 1, _nParametersPerGroup, _pzero);
    _readKeysDbl("PSCAL", 1, _nParametersPerGroup, _pscal);
    for (unsigned i = 0; i < nAxis; i++) {
        _parameters[_ptype[i]] = i;
    }

    _sortOrder = SORT_TIME_BASELINE;
}



/// Read the UV data section.
void uvfitsLoader::_readData()
{
    int status = 0;

    if (_sortOrder != SORT_TIME_BASELINE)
        throw err("Time Baseline sort order required for the following read.");

    // TODO set start group and end group properly from config options
    // = depends on sort order and time selection.
    long startGroup = 0;
    long endGroup = (_nAntennas * _nAntennas - 1)  / 2;
    long nGroups = endGroup - startGroup + 1;
    Q_ASSERT(startGroup > 0); // groups start at 1

    // Read the visibility amplitudes for the selected configuration
    // one group at a time and copy into the visibility data blob
    unsigned nChan = _endChannel - _startChannel + 1;
    unsigned nPol = _endPolarisation - _startPolarisation + 1;

    std::vector<long> lPixel(_nAxes);
    std::vector<long> fPixel(_nAxes, 1);
    std::vector<long> inc(_nAxes, 1);
    std::vector<int> anyNull(_nAxes, 0);
    lPixel = _naxis;
    float nullVal = 0.0;

    std::vector<float> paramTemp(_nParametersPerGroup);
    unsigned nAmpsPerGroup = nChan * nPol;
    std::vector<float> dataTemp(nAmpsPerGroup);

    // Loop over groups reading parameters and amplitude values.
    for (unsigned g = startGroup; g <= endGroup; g++) {

        // Read the parameters
        ffggpe(_fits, g, 1, _nParametersPerGroup, &paramTemp[0], &status);

        // Read the amplitude values
        ffgsve(_fits, g, _nAxes, &_naxis[0], &fPix[0], &lPix[0], &inc[0],
                nullVal, &amp[0], &status);

        // TODO:
        // save amplitudes and parameters into data blob
    }
}


/**
 * @details
 */
void uvfitsLoader::_open(const QString& fileName)
{
    int status = 0;
    fits_open_file(&_fits, _fileName.toLatin1().data(), READONLY, &status);
    if (status)
        throw err("Problem opening FITS file");
}


/**
 * @details
 */
void uvfitsLoader::_close()
{

}


/**
 * @details
 * Extracts configuration for the fits loader from the configuration node.
 */
void uvfitsLoader::_getConfiguration(const ConfigNode& config)
{
    _fileName = config.getOption("file", "name");
    _startTime = config.getOption("timeIndex", "start", "0").toUInt();
    _endTime = config.getOption("timeIndex", "end", "0").toUInt();
    _startChannel = config.getOption("channelIndex", "start", "0").toUInt();
    _endChannel = config.getOption("channelIndex", "end", "0").toUInt();
    _startPolarisation = config.getOption("polarisationIndex", "start", "0").toUInt();
    _endPolarisation = config.getOption("polarisationIndex", "end", "0").toUInt();
    _pointingRa = config.getOption("pointingIndex", "ra", "0");
    _pointingDec = config.getOption("pointingIndex", "dec", "0");
}



/**
 * @details
 * Read a double type value keyword.
 */
double uvfitsLoader::_readKeyDbl(const QString& key) const
{
    double value = 0.0;
    int status = 0;
    ffgky(_fits, TDOUBLE, key.toLatin1().data(), &value, NULL, status);
    return value;
}


/**
 * @details
 * Read a long type value keyword.
 */
long uvfitsLoader::_readKeyLng(const QString key) const
{
    long value = 0;
    int status = 0;
    ffgky(_fits, TLONG, key.toLatin1().data(), &value, NULL, status);
    return value;
}


/**
 * @details
 * Read a string type value keyword.
 */
QString uvfitsLoader::_readKeyStr(const QString& key) const
{
    char value[FLEN_VALUE];
    int status = 0;
    ffgky(_fits, TSTRING, key.toLatin1().data(), &value, NULL, status);
    return QString(value);
}


/// Read number of long type header keywords.
/// ie. NAXIS<*> keywords
void uvfitsLoader::_readKeysLng(const QString key, const int start,
        const int end, std::vector<long>& values) const
{
    int status = 0;
    int n = 0;
    ffgknj(_fits, key.toLatin1().data(), start, end, &values[0], &n, &status);
}

/**
 * @details
 * Read a set numbered set of of double type value keywords taking with the key
 * pattern keyN where N is a integer between /p start and /p end.
 */
void uvfitsLoader::_readKeysDbl(const QString key, const int start,
        const int end, std::vector<double>& values) const
{
    int status = 0;
    int n = 0;
    ffgknd(_fits, key.toLatin1().data(), start, end, &values[0], &n, &status);
}

/// Read a number of string type header keywords.
void uvfitsLoader::_readKeysStr(const QString key, const int start,
        const int end, std::vector<QString>& values) const
{
    unsigned nValues = end - start + 1;

    // Allocate vector of values to be read
    char** s = (char**) malloc(nValues * sizeof(char*));
    for (unsigned i = 0; i < nValues; i++) {
        s[i] = (char*) malloc(FLEN_VALUE * sizeof(char));
    }

    // Read keys
    int status = 0;
    int nFound = 0;
    ffgkns(_fptr, keyName.toLatin1().data(), start, end, s, &nFound, &status);

    if (nFound != nValues)
        throw QString("uvfitsLoader::_readKeysStr(): Not enough keys found.");

    // Copy values into vector of QStrings (return vector)
    for (int i = 0; i < nValues; i++) {
        values[i] = QString(s[i]);
    }

    // Clean up memory
    for (unsigned i = 0; i < nValues; i++) free(s[i]);
    free(s);
}


/**
 * @details
 * Constructs a standard throw message for the class.
 */
QString uvfitsLoader::err(const QString& text)
{
    return "uvfitsLoader: " + text;
}

} // namespace pelican
