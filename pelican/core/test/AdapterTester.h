#ifndef ADAPTERTESTER_H
#define ADAPTERTESTER_H

#include <QtCore/QString>
#include <QtCore/QHash>
#include "pelican/utility/FactoryConfig.h"
#include "pelican/utility/Config.h"
#include "pelican/core/AbstractAdapter.h"

class QIODevice;

/**
 * @file AdapterTester.h
 */

namespace pelican {
class DataBlob;
namespace test {


/**
 * @class AdapterTester
 *
 * @brief
 *     Helper for writing Unit tests for Adapters
 * @details
 *
 */
class AdapterTester
{
    public:
        /// AdapterTester constructor.
        AdapterTester(const QString& name, const QString& xml);

        /// AdapterTester destructor.
        virtual ~AdapterTester();

        /// set a data file from which to read in test data
        //  the file is assumed to exist in the data directory
        //  of the test folder
        void setDataFile(const QString& filename);

        /// set any supporting service data
        void setServiceData(QHash<QString, DataBlob*>& serviceData);

        /// execute
        //  run deserialise() passing the set data stream
        //  with the number of specified bytes
        virtual void execute(DataBlob* blob);

    private:
        FactoryConfig<AbstractAdapter>* _adapterFactory;
        Config  _config;
        QIODevice* _device;
        QHash<QString, DataBlob*> _service;
        AbstractAdapter* _object;
        int _bytes;
};

} // namespace pelican
} // namespace pelican

#endif // ADAPTERTESTER_H
