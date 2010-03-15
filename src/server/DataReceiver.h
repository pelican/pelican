#ifndef DATARECEIVER_H
#define DATARECEIVER_H


#include <QThread>
#include <QString>
class QAbstractSocket;

/**
 * @file DataReceiver.h
 */

namespace pelican {
class DataManager;
class AbstractChunker;

/**
 * @class DataReceiver
 *
 * @brief
 *   Connects incoming data streams to the Chunker inside a separate thread
 * @details
 *
 */

class DataReceiver : public QThread
{
    Q_OBJECT

    public:
        DataReceiver( AbstractChunker* chunker, DataManager* dm, QObject* parent=0 );
        ~DataReceiver();

        /// connect to the specified host and listen
        //  for incoming data
        void listen(const QString& host, quint16 port);

    public slots:
        void die();

    protected slots:
        void processIncomingData();

    protected:
        void run();

    private:
        AbstractChunker* _chunker;
        QAbstractSocket* _socket;
        DataManager* _dm;
        QString _host;
        quint16 _port;

};

} // namespace pelican
#endif // DATARECEIVER_H
