#ifndef WRITABLEDATA_H
#define WRITABLEDATA_H


/**
 * @file WritableData.h
 */

namespace pelican {

class Data;

/**
 * @class WritableData
 *  
 * @brief
 *    Provides locked access to an area of memory
 *    to enable writing
 * @details
 *    The data will become unlocked on destruction
 * 
 */

class WritableData 
{
    public:
        WritableData(Data* d);
        ~WritableData();
        Data* data();

    private:
        Data* _data;
};

} // namespace pelican
#endif // WRITABLEDATA_H 
