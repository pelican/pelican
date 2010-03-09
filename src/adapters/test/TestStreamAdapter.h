#ifndef TESTSTREAMADAPTER_H
#define TESTSTREAMADAPTER_H


#include "AbstractStreamAdapter.h"

/**
 * @file TestStreamAdapter.h
 */

namespace pelican {

/**
 * @class TestStreamAdapter
 *  
 * @brief
 * 
 * @details
 * 
 */

class TestStreamAdapter : public AbstractStreamAdapter
{
    public:
        TestStreamAdapter( const ConfigNode& = ConfigNode() );
        ~TestStreamAdapter();
        virtual void deserialise(QIODevice* in);

    private:
};

} // namespace pelican
#endif // TESTSTREAMADAPTER_H 
