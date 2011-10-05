#ifndef TYPECOUNTER_H
#define TYPECOUNTER_H

#include <QtCore/QList>

/**
 * @file TypeCounter.h
 */

namespace pelican {

/**
 * @class TypeCounter
 *  
 * @brief
 *     maintains containers of values associated with a 
 *     specific type. Returns the max, min values as
 *     the contributors to the type are dynamically added
 *     or removed
 * @details
 *     Uses the inserter sort algoritm, so that computation
 *     overhead is done only when adding() data
 */

template<typename T>
class TypeCounter
{
    public:
        TypeCounter() {};
        ~TypeCounter() {};

        // return true if there is no data
        bool isEmpty() const { return _data.isEmpty(); }

        // remove a value from the data
        void remove(const T& d) { _data.removeOne(d); }

        // add a value to the data
        void add(const T& t) {
            // Inserter directly into the correct place
            int j=0; 
            while( j < _data.size() && _data[j] < t) {
                ++j;
            }
            _data.insert(j, t);
        }

        // return the maximum value. Must check with isEmpty() first
        const T& max() const { return _data.last(); }
        // return the minimum value. Must check with isEmpty() first
        const T& min() const { return _data.first(); }

    private:
        QList<T> _data;
};

} // namespace pelican
#endif // TYPECOUNTER_H 
