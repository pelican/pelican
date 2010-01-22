#ifndef ABSTRACTCHUNKER_H
#define ABSTRACTCHUNKER_H
#include <QUdpSocket>


/**
 * @file AbstractChunker.h
 */

namespace pelican {

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
        AbstractChunker();
        virtual ~AbstractChunker();
        /// create a new socket appropriate to the type expected
        //  on the data stream
        //  by default this will be a UDP socket
        virtual QAbstractSocket* newSocket() const;

        ///  called whenever there is data ready to be processed
        //   Inheriting classes should process a complete data chunk
        //   inside this method
        virtual void next(QAbstractSocket*) = 0;

    private:
};

} // namespace pelican
#endif // ABSTRACTCHUNKER_H 
