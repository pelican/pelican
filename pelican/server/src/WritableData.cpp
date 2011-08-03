#include <string>
#include "pelican/comms/DataChunk.h"
#include "pelican/server/WritableData.h"
#include "pelican/server/AbstractLockableData.h"

namespace pelican {

// class WritableData
WritableData::WritableData(AbstractLockableData* data)
    : _data(data)
{
    if (_data) _data->writeLock();
}

WritableData::~WritableData()
{
    if (_data) _data->writeUnlock();
}

void WritableData::write(const void* buf, size_t size, size_t offset)
{
    if (size + offset > _data->data()->size() )
        throw QString("WritableData::write(): Write overflow!");

    void* mem = (char*)(_data->data()->data()) + offset;
    memcpy(mem, buf, size);
}


WritableData& WritableData::operator=(const WritableData& other)
{
    // Protect against invalid self-assignment.
    if (this != &other) {
        if (other.data()) {
            _data = other.data();
            _data->writeLock();
        }
    }
    return *this;
}

} // namespace pelican
