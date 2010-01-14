#ifndef DATABLOB_H
#define DATABLOB_H

/**
 * @file DataBlob.h
 */

namespace pelican {

/**
 * @class DataBlob
 *  
 * @brief
 * Base class for all Pelican data blobs.
 * 
 * @details
 * This is the base class used for all Pelican data blobs required by
 * pipeline modules. It provides a timestamp based using the Modified
 * Julian Date at the time of creation.
 */
class DataBlob
{
    public:
        /// Data blob constructor.
        DataBlob();

        /// Data blob destructor.
        ~DataBlob();

        /// Sets the timestamp using the current value of the system clock.
        void setTimeStamp();

        /// Returns the MJD timestamp of the data blob.
        double timeStamp() const { return _modifiedJulianDate; }

    private:
        /// The timestamp of the data blob, stored as a modified Julian
        /// date (MJD). Note that MJD = JD - 2400000.5, and starts
        /// from midnight rather than midday.
        double _modifiedJulianDate;
};

} // namespace pelican

#endif // DATABLOB_H 
