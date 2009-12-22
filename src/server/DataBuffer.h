#ifndef DATABUFFER_H
#define DATABUFFER_H
#include <QObject>


/**
 * @file DataBuffer.h
 */

namespace pelican {

/**
 * @class DataBuffer
 *  
 * @brief
 * 
 * @details
 * 
 */

class DataBuffer : public QObject
{
    Q_OBJECT

    public:
        DataBuffer(QObject* parent=0);
        ~DataBuffer();

    private:
};

} // namespace pelican
#endif // DATABUFFER_H 
