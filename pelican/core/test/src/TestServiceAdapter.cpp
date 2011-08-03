#include "TestServiceAdapter.h"
#include "pelican/data/test/TestDataBlob.h"

namespace pelican {
namespace test {

/**
 * @details
 * Deserialises the data from the given QIODevice into a
 * TestDataBlob structure.
 *
 * @param[in] in    A pointer to the input device.
 */
void TestServiceAdapter::deserialise(QIODevice* in)
{
    TestDataBlob* blob = static_cast<TestDataBlob*>(_data);
    blob->resize(_chunkSize);
    in->read( blob->data().data(), (quint64)_chunkSize);
}

} // namespace test
} // namespace pelican
