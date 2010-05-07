#ifndef DATABLOB_H
#define DATABLOB_H

#include "pelican/utility/Factory.h"
#include <QString>
#include <complex>
class QByteArray;

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
 *
 * @note
 * The macro expands to declare a dummy variable of the object's generator
 * type, which (when constructed), adds the type name to the creator's known
 * object types.
 *
 * The macro is placed within the global scope so that it is initialised as
 * soon as the program starts executing. It is placed within an anonymous
 * namespace so that the dummy creator variable is not accessible from outside
 * the file that instantiated it.
 */
#define PELICAN_DECLARE_DATABLOB(type) namespace {CreatorBasic<type, DataBlob> reg(#type);}

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

        /// Serialise the DataBlob into a byte array.
        virtual QByteArray serialise() const;

        /// Serialise the data blob to a data stream.
        virtual void serialise(QDataStream& stream);

        /// Deserialises the DataBlob from the byte array created with serialise.
        virtual void deserialise(const QByteArray&);

        /// Deserialises the data blob to a data stream.
        virtual void deserialise(const QDataStream&);
};

} // namespace pelican

#endif // DATABLOB_H
