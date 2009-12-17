#ifndef DATA_H
#define DATA_H

#include <cstring>

/**
 * @file Data.h
 */

namespace pelican {

/**
 * @class Data
 *  
 * @brief
 *    Primary interface to access Chunks of data in the server
 * @details
 *    This class takes care of locking etc.
 */

class Data
{
    public:
        Data(char* data, size_t size);
        ~Data();
        size_t size() const;
        char* operator*();

    private:
        char* _data;
        size_t _size;
};

} // namespace pelican
#endif // DATA_H 
