#ifndef ABSTRACTLOCKABLEDATA_H
#define ABSTRACTLOCKABLEDATA_H


#include <boost/shared_ptr.hpp>
#include "AbstractLockable.h"

/**
 * @file AbstractLockableData.h
 */

namespace pelican {
    class Data;

/**
 * @class AbstractLockableData
 *  
 * @brief
 *    Interface for data classes that can be locked
 * @details
 * 
 */

class AbstractLockableData : public AbstractLockable
{
    Q_OBJECT

    public:
        AbstractLockableData( quint64 maxSize, QObject* parent );
        virtual ~AbstractLockableData();

        /// return the data id 
        QString id() const;

        /// set the object identity
        void setId(const QString&);

        /// return the underlying data object
        boost::shared_ptr<Data> data() const { return _data; } ;

        /// Return the maximum size.
        quint64 maxSize() {return _maxSize;}

    protected:
        boost::shared_ptr<Data> _data;

    private:
        quint64 _maxSize;
};

} // namespace pelican
#endif // ABSTRACTLOCKABLEDATA_H 
