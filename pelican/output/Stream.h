#ifndef STREAM_H
#define STREAM_H

/**
 * @file Stream.h
 */

#include <boost/shared_ptr.hpp>
#include <QtCore/QString>

namespace pelican {

class DataBlob;

/**
 * @ingroup c_output
 *
 * @class Stream
 *
 * @brief
 *   Contains info about a stream
 * @details
 *
 */
class Stream
{
    public:
        /// Stream constructor.
        Stream(const QString& streamName = QString() );

        /// return the name of the stream
        const QString& name() const { return _name; }

        /// return the data type of the stream
        const QString& type() const;

        /// return a shared ptr to the data
        boost::shared_ptr<DataBlob> data() const;

        /// set the data
        void setData( const boost::shared_ptr<DataBlob>& );

        /// Stream destructor.
        ~Stream();

    private:
        QString _type;
        QString _name;
        boost::shared_ptr<DataBlob> _data;

};

} // namespace pelican

#endif // STREAM_H
