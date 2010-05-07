#ifndef TELESCOPEEMULATOR_H_
#define TELESCOPEEMULATOR_H_

/**
 * @file TelescopeEmulator.h
 */


#include <QThread>
#include <QHostAddress>

namespace pelican {

/**
 * @class TelescopeEmulator
 *
 * @brief
 * Test class to emulate UDP signals sent from a telescope for testing the
 * server.
 *
 * @details
 *
 */
class TelescopeEmulator : public QThread
{
    Q_OBJECT

    public:
        /// Constructs a new telescope emulator.
        TelescopeEmulator(const double initialValue = 0.0,
                const qint16 port = 8080,
                const QHostAddress host = QHostAddress::LocalHost,
                const int interval = 1000,
                const int chunkSize = 512);

        /// Destroys the telescope emulator.
        ~TelescopeEmulator();

    public:
        /// Runs the telescope emulator thread.
        void run();

    private:
        bool _abort;
        QHostAddress _host;
        qint16 _port;
        int _interval;
        int _chunkSize;
        double _initialValue;
};

} // namespace pelican

#endif // TELESCOPEEMULATOR_H_
