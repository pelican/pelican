#ifndef ADAPTEREXAMPLE_H
#define ADAPTEREXAMPLE_H

#include "pelican/adapters/AbstractStreamAdapter.h"

using namespace pelican;

/**
 * @class AdapterExample
 *  
 * @brief
 *    An example of a Stream Adapter class
 */

class AdapterExample : public AbstractStreamAdapter
{
    public:
        AdapterExample(const ConfigNode& config);
        virtual ~AdapterExample();

    private:
};

#endif // ADAPTEREXAMPLE_H 
