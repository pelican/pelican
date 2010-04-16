#include "pelican/data/DataBlob.h"
#include <QDate>
#include <QTime>
#include <QDateTime>

namespace pelican {

/**
 * @details
 * Constructs a new data blob.
 */
DataBlob::DataBlob()
{
    // Set the default time stamp.
    setTimeStamp();
}

/**
 * @details
 * Data blob destructor.
 */
DataBlob::~DataBlob()
{
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

} // namespace pelican
