#ifndef TELESCOPEEMULATOR_H_
#define TELESCOPEEMULATOR_H_

/**
 * @file TelescopeEmulator.h
 */


#include <QThread>

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
        TelescopeEmulator(const qint16 port, const double initialValue = 0.0);

        /// Destroys the telescope emulator.
        ~TelescopeEmulator();

    public:
        /// Runs the telescope emulator thread.
        void run();

    private:
        bool _abort;
        qint16 _port;
        double _initialValue;
};

} // namespace pelican

#endif // TELESCOPEEMULATOR_H_
