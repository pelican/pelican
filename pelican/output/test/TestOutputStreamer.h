#ifndef TESTOUTPUTSTREAMER_H
#define TESTOUTPUTSTREAMER_H

/**
 * @file TestOutputStreamer.h
 */

#include "pelican/output/AbstractOutputStream.h"
#include "pelican/utility/ConfigNode.h"
#include <QtCore/QString>
#include <QtCore/QMap>

namespace pelican {
class DataBlob;

namespace test {

/**
 * @ingroup t_output
 *
 * @class TestOutputStreamer
 *
 * @brief
 * An implementation of AbstractOutputStream for testing
 *
 * @details
 */

class TestOutputStreamer : public AbstractOutputStream
{
    public:
        TestOutputStreamer( const QString& id="TestOutputStreamer",
                const ConfigNode& config = ConfigNode());
        TestOutputStreamer( const ConfigNode& config );
        ~TestOutputStreamer();
        const QString& id() const { return _id; };
        const DataBlob* lastReceived( const QString& streamName );

    protected:
        virtual void sendStream(const QString& streamName, const DataBlob* dataBlob);

    private:
        QString _id;
        ConfigNode _config;
        QMap<QString, const DataBlob* > _received;
};

PELICAN_DECLARE(AbstractOutputStream, TestOutputStreamer)

} // namespace test
} // namespace pelican
#endif // TESTOUTPUTSTREAMER_H
