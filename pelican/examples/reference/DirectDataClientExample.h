#ifndef DIRECTDATACLIENTEXAMPLE_H
#define DIRECTDATACLIENTEXAMPLE_H


#include "pelican/core/DirectStreamDataClient.h"

/**
 * @file DirectDataClientExample.h
 */

using namespace pelican;

/**
 * @ingroup c_examples
 *
 * @class DirectDataClientExample
 *
 * @brief
 *    An example of a class inheriting the DirecetDataClient
 *    and installing two instances of ChunkerExample, and
 *    assigning them to two differently named streams
 * @details
 *
 */
class DirectDataClientExample : public DirectStreamDataClient
{
    public:
        /// DirectDataClientExample constructor.
        DirectDataClientExample( const ConfigNode& configNode,
                 const DataTypes& types, const Config* config );

        /// DirectDataClientExample destructor.
        ~DirectDataClientExample();

    private:
};

PELICAN_DECLARE_CLIENT(DirectDataClientExample)

#endif // DIRECTDATACLIENTEXAMPLE_H
