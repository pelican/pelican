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
 * TestClass to emulate UDP signals sent from a telescope for testing the
 * server.
 *
 * @details
 *
 */

class TelescopeEmulator : public QThread
{
    public:
        TelescopeEmulator(const qint16 port, const double initialValue = 0.0);
        virtual ~TelescopeEmulator();

    public:
        ///
        void run();

    private:
        bool _abort;
        qint16 _port;
        double _initialValue;
};

} // namespace pelican

#endif // TELESCOPEEMULATOR_H_
