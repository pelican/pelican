#include "pelican/data/DataBlob.h"
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>
#include <QtCore/QIODevice>

namespace pelican {

/**
 * @details
 * Constructs a new data blob, setting the type-name
 * and the time stamp at the time of creation using the system clock.
 *
 * @param[in] type The name of the data blob derived class.
 */
DataBlob::DataBlob(const QString& type) : _type(type)
{
    setTimeStamp();
}

/**
 * @details
 * Sets the timestamp of the data blob using the Modified Julian Date
 * from the system clock, converted to UTC.
 * Note that MJD = JD - 2400000.5, and starts from midnight
 * rather than midday.
 */
void DataBlob::setTimeStamp()
{
    // Get the system date.
    QDateTime utc = QDateTime::currentDateTime().toUTC();

    // Set the date part of the MJD.
    _modifiedJulianDate = utc.date().toJulianDay();
    _modifiedJulianDate -= 2400000.5;

    // Set the time part of the MJD.
    QTime midnight(0, 0, 0);
    double millisec = midnight.msecsTo(utc.time());
    _modifiedJulianDate += (millisec / 86400000.0);
}


/**
 * @details
 * Serialises the DataBlob into the QIODevice.
 * This method should be reimplemented in a derived class if needed,
 * since the default implementation will throw an exception of type QString.
 */
void DataBlob::serialise(QIODevice&) const
{
    throw QString("DataBlob: This object's ('%1') serialise method "
            "is undefined.").arg(type());
}

/**
 * @details
 * This method must return the number of bytes saved to the QIODevice
 * for the serialised data blob when calling the serialise() method.
 */
quint64 DataBlob::serialisedBytes() const
{
    throw QString("DataBlob: This object's ('%1') serialisedBytes method "
            "is undefined.").arg(type());
}

/**
 * @details
 * Deserialises the data blob.
 * This method should be reimplemented in a derived class if needed,
 * since the default implementation will throw an exception of type QString.
 */
void DataBlob::deserialise(QIODevice&, QSysInfo::Endian)
{
    throw QString("DataBlob: This object's ('%1') deserialise method "
            "is undefined.").arg(type());
}

} // namespace pelican
