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
 * Connects incoming data streams to the Chunker inside a separate thread.
 *
 * @details
 *
 */
class DataReceiver : public QThread
{
    Q_OBJECT

    public:
        /// Constructor.
        DataReceiver( AbstractChunker* chunker, DataManager* dm, QObject* parent=0 );

        /// Destructor.
        ~DataReceiver();

        /// Connect to the specified host and listen for incoming data.
        void listen(const QString& host, quint16 port);

    protected slots:
        /// TODO Write brief description.
        void processIncomingData();

    protected:
        /// Creates and runs the thread for the data receiver.
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
