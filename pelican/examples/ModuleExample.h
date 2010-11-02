#ifndef MODULEEXAMPLE_H
#define MODULEEXAMPLE_H

#include "pelican/modules/AbstractModule.h"
#include "pelican/examples/DataBlobExample.h"

/**
 * @file ModuleExample.h
 */

using namespace pelican;

/**
 * @ingroup c_examples
 *
 * @class ModuleExample
 *
 * @brief
 * A simple example to demonstrate how to write a pipeline module.
 */

/*
 * A simple example to demonstrate how to write a pipeline module.
 */
class ModuleExample : public AbstractModule
{
    public:
        ModuleExample(const ConfigNode& config);
        void run(const DataBlobExample* input1,
                const DataBlobExample* input2, DataBlobExample* output);

    private:
        typedef enum {OpNone, OpAdd, OpMultiply} Operation;
        Operation optype;
};

// Register the module.
PELICAN_DECLARE_MODULE(ModuleExample)

#endif // MODULEEXAMPLE_H
