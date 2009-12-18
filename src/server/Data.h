#ifndef DATA_H
#define DATA_H

#include <QMutex>
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
        void lock();
        void unlock();

    protected:
        QMutex _mutex;

    private:
        char* _data;
        size_t _size;
        int _lock;
};

} // namespace pelican
#endif // DATA_H 
