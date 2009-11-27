#ifndef SESSION_H
#define SESSION_H

#include <QThread>

/**
 * @file Session.h
 */

namespace pelican {

/**
 * @class Session
 *  
 * @brief
 *    Class to process a single server request
 * @details
 * 
 */

class Session : public QThread
{
    Q_OBJECT

    public:
        Session(int socketDescriptor,QObject* parent=0 );
        ~Session();

    private:
};

} // namespace pelican
#endif // SESSION_H 
