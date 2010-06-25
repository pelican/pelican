#include "pelican/data/DataBlob.h"
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>
#include <QtCore/QIODevice>

namespace pelican {

/**
 * @details
 * Constructs a new data blob.
 */
DataBlob::DataBlob(const QString& type) : _type(type)
{
    // Set the default time stamp.
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
 * Serialises the data blob.
 */
void DataBlob::serialise(QIODevice&) const
{
    throw QString("DataBlob: This object's ('%1') serialise method "
            "is undefined.").arg(type());
}


/**
 * @details
 * Deserialises the data blob.
 */
void DataBlob::deserialise(QIODevice&)
{
    throw QString("DataBlob: This object's ('%1') deserialise method "
            "is undefined.").arg(type());
}

} // namespace pelican
