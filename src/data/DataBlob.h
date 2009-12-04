#ifndef DATABLOB_H
#define DATABLOB_H


/**
 * @file DataBlob.h
 */

/**
 * @class DataBlob
 *  
 * @brief
 * 
 * @details
 * 
 */

namespace pelican {

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
