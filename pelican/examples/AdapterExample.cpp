#include "AdapterExample.h"

/**
 *@details AdapterExample 
 * You will be passed a config node in the constructor
 * to help you set the Adapter up.
 * You can specify whatever requirements you want in your
 * config
 *
 */
AdapterExample::AdapterExample(const ConfigNode& config)
    : AbstractStreamAdapter(config)
{
}

AdapterExample::~AdapterExample()
{
}

