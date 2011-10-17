#ifndef CIRCULARBUFFERITERATOR_H
#define CIRCULARBUFFERITERATOR_H

#include <QtCore/QList>

/**
 * @file CircularBuffer.h
 */

namespace pelican {

/**
 * @class CircularBuffer
 *  
 * @brief
 *   A generic class for iterating over a QList
 *   in a circular fashion.
 * @details
 * 
 */

template<typename T>
class CircularBufferIterator
{

    public:
        CircularBufferIterator( QList<T>* buffer )
                : _index(-1) { reset(buffer); };

        /// refresh to the new buffer
        /// The index will be moved to point to beginning
        void reset( QList<T>* buffer ) {
             _data = buffer;
             _size = _data->size();
             _index = -1;
        }
        ~CircularBufferIterator() {};

        /// reset the index to point to the position in the
        /// buffer containing the provided value
        /// If this value does not exist, the index will reset
        /// to the beginning of the buffer
        /// note that after this call, the operator* can become
        /// invalid until next() has been called.
        void resetIndex( const T& value ) {
            _index = _data->indexOf(value);
        }

        /// get the next item from the buffer
        T& next() {
            _index=++_index%_size;
            return (*_data)[_index];
        };

        /// return the value currently pointed to
        //  undefined before next() has been called
        T& operator*() { return (*_data)[_index]; }

    private:
        QList<T>* _data;
        unsigned int _index;
        unsigned int _size;
};

} // namespace pelican
#endif // CIRCULARBUFFERITERATOR_H 
