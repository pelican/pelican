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
        TelescopeEmulator();
        virtual ~TelescopeEmulator();

    public:
        ///
        void run();

    private:
        bool _abort;
};

} // namespace pelican

#endif // TELESCOPEEMULATOR_H_
