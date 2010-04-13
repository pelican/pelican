#ifndef ABSTRACTLOCKABLEDATA_H
#define ABSTRACTLOCKABLEDATA_H

/**
 * @file AbstractLockableData.h
 */

#include <boost/shared_ptr.hpp>
#include "pelican/server/AbstractLockable.h"
#include "pelican/comms/Data.h"

namespace pelican {

class Data;

/**
 * @class AbstractLockableData
 *
 * @brief
 * Interface for data classes that can be locked.
 *
 * @details
 * TODO Detailed description for AbstractLockableData.
 */
class AbstractLockableData : public AbstractLockable
{
    Q_OBJECT

    protected:
        boost::shared_ptr<Data> _data;

    private:
        quint64 _maxSize;

    public:
        /// Constructs a new AbstractLockableData object.
        AbstractLockableData( quint64 maxSize, QObject* parent )
        : AbstractLockable(parent), _maxSize(maxSize) {}

        /// Destroys the AbstractLockableData object.
        virtual ~AbstractLockableData() {}

        /// Return the data ID.
        QString id() const {return _data->id();}

        /// Set the object identity.
        void setId(const QString& id) {_data->setId(id);}

        /// Return the underlying data object.
        boost::shared_ptr<Data> data() const {return _data;}

        /// Return the maximum size.
        quint64 maxSize() {return _maxSize;}
};

} // namespace pelican

#endif // ABSTRACTLOCKABLEDATA_H
