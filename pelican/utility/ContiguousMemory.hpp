#ifndef CONTIGUOUSMEMORY_H
#define CONTIGUOUSMEMORY_H
#include <memory>
#include <QtCore/QList>

/**
 * @file ContiguousMemory.h
 */

namespace pelican {

/**
 * @class ContiguousMemory
 *  
 * @brief
 *    A C++ interface to the std::get_temporary_buffer
 * @details
 * 
 */

template<typename T>
class ContiguousMemory
{
    private:
        T* _buf;
        QList<unsigned int> _free;
        unsigned long _size;

    public:
        // reserve a contiguous memory buffer for the specified number
        // of objects
        ContiguousMemory( unsigned long num ) : _size(num) { 
            // allocate the memory
            std::pair<T*,unsigned long> r = std::get_temporary_buffer<T>(num);
            if( r.second != num ) throw(QString("ContiguousMemory: unable to allocate num objects"));
            _buf = r.first;
            // mark all slots as free
            for(unsigned int i=0; i<_size; ++i ) {
                _free.append(i);
            }
        };
        ~ContiguousMemory() { std::return_temporary_buffer<T>(_buf); }

        // get a pointer to a free memory slot - or NULL if no memory available
        T* nextFree() { 
           if(_free.size() == 0) return NULL;
           return &_buf[_free.takeFirst()];
        }

        /// recover memory in the buffer for the specified object
        void free( T* object ) { 
            Q_ASSERT( object >= _buf && object <= _buf+(_size*sizeof(T))); 
            _free.append( object - _buf );
        }

};

} // namespace pelican
#endif // CONTIGUOUSMEMORY_H 
