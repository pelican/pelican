#ifndef DATABLOB_H
#define DATABLOB_H

#include <QString>
#include <complex>

/**
 * @file DataBlob.h
 */

namespace pelican {

/// Real floating point data type.
typedef double real_t;

/// Complex floating point data type.
typedef std::complex<real_t> complex_t;

/**
 * @class DataBlob
 *  
 * @brief
 * Base class for all Pelican data blobs.
 * 
 * @details
 * This is the base class used for all Pelican data blobs required by
 * pipeline modules. It provides a timestamp using the Modified
 * Julian Date at the time of creation.
 */
class DataBlob
{
    private:
        /// The timestamp of the data blob, stored as a modified Julian
        /// date (MJD). Note that MJD = JD - 2400000.5, and starts
        /// from midnight rather than midday.
        double _modifiedJulianDate;
        QString _version;

    public:
        /// Data blob constructor.
        DataBlob();

        /// Data blob destructor.
        virtual ~DataBlob();

        /// Sets the time stamp using the current value of the system clock.
        void setTimeStamp();

        /// Sets the time stamp to the given value.
        void setTimeStamp(double timeStamp) {_modifiedJulianDate = timeStamp;}

        /// Returns the MJD timestamp of the data blob.
        double timeStamp() const { return _modifiedJulianDate; }

        /// set the version id of the DataBlob
        void setVersion(const QString& v) { _version = v; };

        /// Returns the version of the DataBlob
        QString version() const { return _version; };
};

} // namespace pelican

#endif // DATABLOB_H 
