#ifndef ABSTRACTCHUNKER_H
#define ABSTRACTCHUNKER_H
#include <QUdpSocket>
#include <QString>

/**
 * @file AbstractChunker.h
 */

namespace pelican {
class DataManager;
class WritableData;

/**
 * @class AbstractChunker
 *  
 * @brief
 *    Base class for all chunker plugins
 * @details
 *    Methods on this class are called by the DataListener class
 *    which sets up the necessary connections etc.
 */

class AbstractChunker
{

    public:
        AbstractChunker(DataManager*);
        virtual ~AbstractChunker();
        /// create a new socket appropriate to the type expected
        //  on the data stream
        //  by default this will be a UDP socket
        virtual QAbstractSocket* newSocket() const;

        ///  called whenever there is data ready to be processed
        //   Inheriting classes should process a complete data chunk
        //   inside this method
        virtual void next(QAbstractSocket*)  = 0;

    protected:
        /// access to memory to store data is through this interface.
        //  The WritableData object should always be checked with its isValid() method
        //  before use. When the WritableData object goes out of scope the data 
        //  will become available to be served automatically if it is valid.
        WritableData getDataStorage(const QString& type, size_t size ) const;

    private:
        DataManager* _dm;
};

} // namespace pelican
#endif // ABSTRACTCHUNKER_H 
