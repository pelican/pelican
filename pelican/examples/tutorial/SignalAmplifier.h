#ifndef SIGNALAMPLIFIER_H
#define SIGNALAMPLIFIER_H

#include "pelican/core/AbstractModule.h"

using namespace pelican;

class SignalData;

/*
 * A simple example to demonstrate how to write a pipeline module.
 */
class SignalAmplifier : public AbstractModule
{
    public:
        // Constructs the module.
        SignalAmplifier(const ConfigNode& config);

        // Runs the module.
        void run(const SignalData* input, SignalData* output);

    private:
        float gain;
};

PELICAN_DECLARE_MODULE(SignalAmplifier)

#endif // SIGNALAMPLIFIER_H
