#ifndef TESTADAPTER_H
#define TESTADAPTER_H


#include "AbstractServiceAdapter.h"
#include "utility/ConfigNode.h"

/**
 * @file TestAdapter.h
 */

namespace pelican {

/**
 * @class TestAdapter
 *  
 * @brief
 *    Simple Pass Through Adapter for use with the TestBataBlob object
 * @details
 * 
 */

class TestAdapter : public AbstractServiceAdapter
{
    public:
        TestAdapter( const ConfigNode& = ConfigNode() );
        ~TestAdapter();
        virtual void deserialise(QIODevice* in);

    private:
};

} // namespace pelican
#endif // TESTADAPTER_H 
