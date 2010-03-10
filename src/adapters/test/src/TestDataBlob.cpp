#include "TestDataBlob.h"
#include <malloc.h>


#include "utility/memCheck.h"

namespace pelican {


/**
 *@details TestDataBlob 
 */
TestDataBlob::TestDataBlob()
    : DataBlob()
{
}

/**
 *@details
 */
TestDataBlob::~TestDataBlob()
{
}

void TestDataBlob::resize(size_t size)
{
    _buf.resize(size + 1);
    _buf[size+1] = '\0'; // add a safety if someone tries to print
}

char* TestDataBlob::data()
{
    return &_buf[0];
}

} // namespace pelican
