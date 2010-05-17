#ifndef DATABLOB_H
#define DATABLOB_H

#include <QString>
#include <complex>
#include "pelican/utility/FactoryRegistrar.h"
class QIODevice;

/**
 * @file DataBlob.h
 */

namespace pelican {

/// Real floating point data type.
typedef double real_t;

/// Complex floating point data type.
typedef std::complex<real_t> complex_t;

/**
 * This macro is used to register the named data blob type with the factory.
 * It should be used within the global scope of the data blob source file.
 */
#define PELICAN_DECLARE_DATABLOB(type) PELICAN_DECLARE(DataBlob, type)

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
        QString _type;

    public:
        /// Data blob constructor.
        PELICAN_CONSTRUCT_TYPES_EMPTY
        DataBlob(const QString& type = QString("") );

        /// Data blob destructor.
        virtual ~DataBlob();

        /// return a QString of the type of DataBlob (should be the same as the class name)
        QString type() const;

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

        /// Serialise the DataBlob into the QIODevice.
        virtual void serialise(QIODevice&) const;

        /// Deserialises the DataBlob from the QIODevice
        virtual void deserialise(QIODevice&);
};

} // namespace pelican

#endif // DATABLOB_H
