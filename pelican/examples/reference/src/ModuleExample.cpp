#include "reference/ModuleExample.h"
#include "reference/DataBlobExample.h"
#include "pelican/utility/Config.h"

// Construct the example module.
ModuleExample::ModuleExample(const ConfigNode& config)
    : AbstractModule(config)
{
    // Initialise.
    optype = OpNone;

    // Set operation type from the configuration.
    QString type = config.getOption("operation", "type").toLower();
    if (type == "add")
        optype = OpAdd;
    else if (type == "multiply")
        optype = OpMultiply;
}

// Runs the module.
void ModuleExample::run(const DataBlobExample* input1,
        const DataBlobExample* input2, DataBlobExample* output)
{
    // Ensure both input data blobs match in dimensions.
    if (input1->size() != input2->size())
        throw QString("ModuleExample::run(): Input data dimension mismatch.");

    // Ensure the output storage data is big enough.
    unsigned nPts = input1->size();
    if (output->size() != nPts)
        output->resize(nPts);

    // Get pointers to the memory to use.
    const float* in1 = input1->data();
    const float* in2 = input2->data();
    float* out = output->data();

    // Perform the operation.
    if (optype == OpAdd) {
        for (unsigned i = 0; i < nPts; ++i) {
            out[i] = in1[i] + in2[i];
        }
    }
    else if (optype == OpMultiply) {
        for (unsigned i = 0; i < nPts; ++i) {
            out[i] = in1[i] * in2[i];
        }
    }
}
