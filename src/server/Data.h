#ifndef DATA_H
#define DATA_H

#include <cstring>
#include <QMutex>

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
        Data(char* data=0, size_t size=0);
        ~Data();
        size_t size() const;
        char* operator*();
        void lock();
        void unlock();
        bool isValid() const;

    protected:
        QMutex _mutex;

    private:
        Data(const Data&); // disallow the copy constructor

    private:
        char* _data;
        size_t _size;
        int _lock;
};

} // namespace pelican
#endif // DATA_H 
